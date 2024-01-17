/*
    DataBlockBuff.h - Service to allocate, maintain and exchange blocks of (opaque) data

    Copyright 2024,  Ichthyostega

    This file is part of yoshimi, which is free software: you can redistribute
    it and/or modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the License,
    or (at your option) any later version.

    yoshimi is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  See the GNU General Public License (version 2
    or later) for more details.

    You should have received a copy of the GNU General Public License
    along with yoshimi.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef DATA_BLOCK_BUFF_H
#define DATA_BLOCK_BUFF_H

#include "globals.h"
//#include "Misc/FormatFuncs.h"

//#include <functional>
//#include <string>
#include <cassert>
#include <chrono>
#include <array>

using std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<steady_clock>;

/**
 * Uninitialised memory block
 * @todo C++17 use std::byte and std::launder
 */
template<size_t siz>
class BufferBlock
{
    alignas(size_t) char buffer[siz];
public:
    // Standard layout, trivially constructible and copyable

    void* accessStorage()
    {
        return static_cast<void*>(&buffer);
    }

    template<typename T>
    T& accessAs()
    {
        static_assert(sizeof(T) <= siz, "insufficient storage in BufferBlock");
        return * reinterpret_cast<T*>(&buffer);
    }
};

/**
 * Index entry to organise the contents of the data block ringbuffer
 */
template<class TAG>
struct ItemDescriptor
{
    TimePoint timestamp{};
    TAG tag{};
};

/**
 * A service to manage blocks of data for exchange through a communication protocol.
 * @warning destructor for data blocks in the buffer will *not* be invoked
 */
template<class TAG, size_t cap, size_t siz>
class DataBlockBuff
{
    using Index = std::array<ItemDescriptor<TAG>, cap>;
    using Buffer = std::array<BufferBlock<siz>, cap>;

    Index index;
    Buffer buffer;

    size_t oldest;

    // must not be copied nor moved
    DataBlockBuff(DataBlockBuff &&)                =delete;
    DataBlockBuff(DataBlockBuff const&)            =delete;
    DataBlockBuff& operator=(DataBlockBuff &&)     =delete;
    DataBlockBuff& operator=(DataBlockBuff const&) =delete;

public:
    DataBlockBuff()
        : index{}
        , oldest{0}
        { }

    size_t claimNextBuffer(TAG const& tag)
    {
        index[oldest].timestamp = steady_clock::now();
        index[oldest].tag = tag;
        size_t curr{oldest};
        incWrap(oldest);
        return curr;
    }

    template<typename DAT>
    DAT& accessSlot(size_t idx)
    {
        assert(idx < cap);
        assert(index[idx].tag.template verifyType<DAT>());
        return buffer[idx].template accessAs<DAT>();
    }

    void* accessRawStorage(size_t idx)
    {
        assert(idx < cap);
        return buffer[idx].accessStorage();
    }

private:
    /** increment index, but wrap at array end.
     * @remark using the array cyclically */
    size_t incWrap(size_t idx, size_t inc = 1)
    {
        return (idx + inc) % cap;
    }
};

#endif /*DATA_BLOCK_BUFF_H*/
