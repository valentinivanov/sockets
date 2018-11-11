#ifndef VI_ATOMIC_HASHSET_HPP
#define VI_ATOMIC_HASHSET_HPP

#include "common_defs.h"
#include "paged_array.hpp"
#include "atomic.hpp"

namespace vi
{
    //motivation for such data structure is avoiding memory allocations.
    //we just pre-allocate memory and then just fill it.
    //i'm not sure but possibly locking memory with mlock() in the memory_block could prevent latencies from swapping.
    template<class T>
    struct table_entry
    {
        enum entry_status
        {
            empty = 0
        };

        uint32_t status;
        T payload;

        table_entry() : status(table_entry<T>::empty)
        {
        }
    };

    template<class K, class T, class HK, uint32_t MaxEntries, uint32_t PageSize>
    class atomic_hashset
    {
    private:

        paged_array<table_entry<T>, MaxEntries, PageSize> mStorage;
        uint32_t mSize;

    public:

        atomic_hashset() : mSize(0)
        {
            mStorage.zero_fill();
        }

        uint32_t max_entry_count() const
        {
            return MaxEntries;
        }

        uint32_t size() const
        {
            return mSize;
        }

        uint32_t try_put(const K& key, const T& value)
        {
            uint32_t index = HK::to_index(key);

            if (index < MaxEntries)
            {
                uint32_t entryStatus = atomic_xadd(&(mStorage[index].status), 1);
                if (table_entry<T>::empty == entryStatus)
                {
                    barrier(); //we want changes to be visible to all
                    mStorage[index].payload = value;
                    
                    atomic_add(&mSize, 1);
                }

                return entryStatus;
            }
            else
            {
                return MaxEntries + 1;
            }
        }

        const table_entry<T>& find(const K& key)
        {
            static table_entry<T> empty;

            uint32_t index = HK::to_index(key);

            if (index < mStorage.size())
            {
                return mStorage[index];
            }
            else
            {
                return empty;
            }
        }
    };
};

#endif