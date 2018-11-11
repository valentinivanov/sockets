#ifndef VI_MEMORY_BLOCK_HPP
#define VI_MEMORY_BLOCK_HPP

#include "common_defs.h"
#include "common_inc.h"

namespace vi
{
    template<class T, uint32_t ElementCount>
    class memory_block
    {
    private:

        T* mBlock;

    public:

        memory_block() : mBlock(new T[ElementCount])
        {
            assert(mBlock != NULL);
        }

        void zero_fill()
        {
            assert(mBlock != NULL);
            ::memset(mBlock, 0, sizeof(T) * ElementCount);
        }

        T& operator[] (uint32_t index) 
        {
            assert(mBlock != NULL);
            assert(index < ElementCount);
            return *(mBlock + index);
        }

        uint32_t size() const
        {
            return ElementCount;
        }

        ~memory_block()
        {
            delete[] mBlock;
        }
    };
};

#endif