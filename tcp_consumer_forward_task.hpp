#ifndef VI_TCP_CONSUMER_FORWARD_TASK_HPP
#define VI_TCP_CONSUMER_FORWARD_TASK_HPP

#include "container_impl.h"
#include "tcp_client.hpp"
#include "message_constants.h"

#define FORWARD_BATCH_SIZE 128

namespace vi
{
    class tcp_consumer_forward_task
    {
    private:

        struct message_list_node
        {
            message_list_node* next;
            uint64_t messageId;

            message_list_node() : next(NULL), messageId(0)
            {
            }

            message_list_node(uint64_t m) : next(NULL), messageId(m)
            {
            }
        };

    private:

        container_hashset* mStorage;
        container_queue* mQueue;
        tcp_client* mTcpConsumer;
        bool mTerminationMessage;

        message_list_node* mListHead;
        message_list_node* mListTail;

    public:

        tcp_consumer_forward_task(container_hashset* storage,
                                  container_queue* queue,
                                  tcp_client* tcpConsumer) : mStorage(storage),
                                                             mQueue(queue),
                                                             mTcpConsumer(tcpConsumer),
                                                             mListHead(NULL),
                                                             mListTail(NULL),
                                                             mTerminationMessage(false)
        {
        }

        tcp_consumer_forward_task(const tcp_consumer_forward_task& other) : mStorage(other.mStorage),
                                                                            mQueue(other.mQueue),
                                                                            mTcpConsumer(other.mTcpConsumer),
                                                                            mListHead(NULL),
                                                                            mListTail(NULL),
                                                                            mTerminationMessage(false)
        {
        }

        tcp_consumer_forward_task& operator=(const tcp_consumer_forward_task& other)
        {
            if (this != &other)
            {
                mStorage = other.mStorage;
                mQueue = other.mQueue;
                mTcpConsumer = other.mTcpConsumer;
                mListHead = NULL; //we don't copy it
                mListTail = NULL; //we don't copy it
                mTerminationMessage = false; //we don't copy it
            }

            return *this;
        }

        void begin()
        {
        }

        bool step()
        {
            drain_queue();

            tcp_client::client_result sendResult = tcp_client::ok;
            while (mListHead != NULL && tcp_client::ok == sendResult)
            {
                const table_entry<Message>& entry = mStorage->find(mListHead->messageId);
                if (entry.status != table_entry<Message>::empty)
                {
                    mTerminationMessage = mTerminationMessage || TERMINATION_MESSAGE_TYPE == entry.payload.MessageType;
                    sendResult = mTcpConsumer->send(entry.payload);
                }
                
                if (tcp_client::ok == sendResult)
                {
                    message_list_node* oldHead = mListHead;
                    mListHead = oldHead->next;
                    delete oldHead;
                }
            }

            return !mTerminationMessage || (mQueue->has_tail() || mListHead != NULL); 
        }

        void end()
        {
        }

    private:

        void drain_queue()
        {
            int drainCount = 0;
            while (mQueue->has_tail() && drainCount < FORWARD_BATCH_SIZE)
            {
                uint64_t messageId = mQueue->pop_tail();

                if (NULL == mListHead)
                {
                    mListHead = new message_list_node(messageId);
                    mListTail = mListHead;
                }
                else
                {
                    mListTail->next = new message_list_node(messageId);;
                    mListTail = mListTail->next;
                }

                drainCount++;
            }
        }
    };
};

#endif