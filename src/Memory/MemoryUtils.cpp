//
//  MemoryUtils.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 26/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <pbrlib/Memory/MemoryUtils.hpp>

#include <cassert>
#include <cstdlib>

namespace pbrlib
{
    void* allocAligned(size_t size)
    {
#if defined (_WIN32) || defined (_WIN64)
        return _aligned_malloc(size, PBRLIB_L1_CACHE_LINE_SIZE);
#else
        void* vptr;
        assert(!posix_memalign(&vptr, PBRLIB_L1_CACHE_LINE_SIZE, size));
        return vptr;  
#endif
    }

    void freeAligned(void* ptr)
    {
#if defined (_WIN32) || defined (_WIN64)
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }

    size_t getAlignSize(size_t size)
    {
        constexpr size_t align = alignof(max_align_t);
        return (size + align - 1) & ~(align - 1);
    }
}
