#ifndef VI_PAGED_ARRAY_HPP
#define VI_PAGED_ARRAY_HPP

#include "memory_block.hpp"

namespace vi
{
    template<class T, uint32_t ElementCount, uint32_t PageSize>
    class paged_array
    {
    private:

        memory_block<memory_block<T, PageSize>, ElementCount / PageSize + 1> mPages;

    public:

        paged_array()
        {
            assert(ElementCount > 0);
            assert(PageSize > 0);
        }

        void zero_fill()
        {
            for(uint32_t i = 0; i < mPages.size(); ++i)
            {
                mPages[i].zero_fill();
            }
        }

        uint32_t size() const
        {
            return ElementCount;
        }

        T& operator[](uint32_t index)
        {
            uint32_t pageIndex = index / PageSize;
            uint32_t indexInPage = index % PageSize;
            return mPages[pageIndex][indexInPage];
        }
    };
};

#endif