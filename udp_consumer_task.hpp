#ifndef VI_UDP_CONSUMER_TASK
#define VI_UDP_CONSUMER_TASK

#include "container_impl.h"
#include "udp_multicast_receiver.hpp"
#include "message_constants.h"

namespace vi
{
    class udp_consumer_task
    {
    private:

        container_hashset* mStorage;
        container_queue* mQueue;

        udp_multicast_receiver* mReceiver;

    public:

        udp_consumer_task(container_hashset* storage, 
                          container_queue* queue, 
                          udp_multicast_receiver* receiver) : mStorage(storage), 
                                                              mQueue(queue), 
                                                              mReceiver(receiver)
        {
        }

        udp_consumer_task(const udp_consumer_task& other) : mStorage(other.mStorage), 
                                                            mQueue(other.mQueue), 
                                                            mReceiver(other.mReceiver)
        {
        }

        udp_consumer_task& operator=(const udp_consumer_task& other)
        {
            if (this != &other)
            {
                mStorage = other.mStorage;
                mQueue = other.mQueue;
                mReceiver = other.mReceiver;
            }

            return *this;
        }

        void begin()
        {
        }

        bool step()
        {
            Message m;
            if (mReceiver->receive<Message>(m) == udp_multicast_receiver::receive_ok)
            {
                int entryStatus = mStorage->try_put(m.MessageId, m);
                if (table_entry<Message>::empty == entryStatus &&
                    TARGET_DATA_VALUE == m.MessageData)
                {
                    mQueue->push_head(m.MessageId);
                }
            }

            return m.MessageType != TERMINATION_MESSAGE_TYPE;
        }

        void end()
        {
        }
    };
};

#endif