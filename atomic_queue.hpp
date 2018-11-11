#ifndef VI_ATOMIC_QUEUE_HPP
#define VI_ATOMIC_QUEUE_HPP

#include "common_defs.h"
#include "common_inc.h"
#include "atomic.hpp"

namespace vi
{
    //multiple producers, single reader
    template<class T, uint32_t MaxBufferSize>
    class atomic_queue
    {
    private:

        T mBuffer[MaxBufferSize]; //ring buffer
        uint32_t mReadLocation;
        uint32_t mAvailableDataLocation;
        uint32_t mWriteLocation;

    public:

        atomic_queue() : mReadLocation(0), mAvailableDataLocation(0), mWriteLocation(0)
        {
        }

        bool has_tail() const
        {
            return atomic_read(mAvailableDataLocation) > atomic_read(mReadLocation);
        }

        void push_head(const T& value)
        {
            uint32_t index = normalize(atomic_xadd(&mWriteLocation, 1));

            barrier(); //we want changes to be
            mBuffer[index] = value;

            atomic_xadd(&mAvailableDataLocation, 1);
        }

        const T& pop_tail()
        {
            uint32_t current = normalize(mReadLocation);
            mReadLocation++;

            return mBuffer[current];
        }

    private:

        uint32_t normalize(uint32_t index) const
        {
            return index % MaxBufferSize;
        }
    };
};

#endif