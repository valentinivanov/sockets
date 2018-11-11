#ifndef VI_CONTAINER_IMPL_H
#define VI_CONTAINER_IMPL_H

#include "common_defs.h"
#include "atomic_hashset.hpp"
#include "atomic_queue.hpp"
#include "data_structs.hpp"

#define HIMPL_MAX_ELEMENT_COUNT ((1024 * 1024 * 100) / sizeof(vi::Message))
#define HIMPL_PAGE_SIZE (HIMPL_MAX_ELEMENT_COUNT / 1000)

#define QIMPL_BUFFER_SIZE (1024 * 10)

namespace vi
{
    template<uint32_t MaxEntries>
    struct uint64_hash
    {
        static uint32_t to_index(const uint64_t& key)
        {
            return (uint32_t)(key % MaxEntries);
        }
    };

    typedef atomic_hashset<uint64_t, Message, uint64_hash<HIMPL_MAX_ELEMENT_COUNT>, HIMPL_MAX_ELEMENT_COUNT, HIMPL_PAGE_SIZE> container_hashset;
    typedef atomic_queue<uint64_t, QIMPL_BUFFER_SIZE> container_queue;
};

#endif
    