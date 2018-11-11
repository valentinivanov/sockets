#ifndef VI_COMMON_DATA_STRUCTS
#define VI_COMMON_DATA_STRUCTS

#include "common_defs.h"

namespace vi
{
    struct Message
    {
        uint16_t MessageSize;
        uint8_t MessageType;
        uint64_t MessageId;
        uint64_t MessageData;

        Message() : MessageSize(0), 
                    MessageType(0), 
                    MessageId(0), 
                    MessageData(0)
        {
        }

        Message(const Message& other) : MessageSize(other.MessageSize), 
                                        MessageType(other.MessageType), 
                                        MessageId(other.MessageId), 
                                        MessageData(other.MessageData)
        {
        }

        Message& operator=(const Message& other)
        {
            MessageSize = other.MessageSize;
            MessageType = other.MessageType;
            MessageId = other.MessageId;
            MessageData = other.MessageData;

            return *this;
        }
    };
};

#endif