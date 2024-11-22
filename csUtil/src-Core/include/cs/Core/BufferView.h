/****************************************************************************
** Copyright (c) 2024, Carsten Schmidt. All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
**
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#pragma once

#include <algorithm>
#include <iterator>
#include <limits>
#include <string_view>

#include <cs/Core/Buffer.h>

namespace cs {

  ////// Public //////////////////////////////////////////////////////////////

  class BufferView {
  public:
    using difference_type = Buffer::difference_type;
    using       size_type = Buffer::size_type;
    using      value_type = Buffer::value_type;

    using const_pointer = const value_type *;

    using const_reference = const value_type &;

    using const_iterator = const_pointer;

    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // Constants /////////////////////////////////////////////////////////////

    static constexpr size_type NPOS = std::numeric_limits<size_type>::max();

    // Construction and Assignment ///////////////////////////////////////////

    constexpr BufferView() noexcept
      : _data{nullptr}
      , _size{0}
    {
    }

    constexpr BufferView& operator=(const BufferView& other) noexcept
    {
      if( &other != this ) {
        _data = other._data;
        _size = other._size;
      }

      return *this;
    }

    constexpr BufferView(const Buffer& buffer) noexcept
      : _data{buffer.data()}
      , _size{buffer.size()}
    {
    }

    constexpr BufferView& operator=(const Buffer& buffer) noexcept
    {
      _data = buffer.data();
      _size = buffer.size();

      return *this;
    }

    BufferView(const void *data, const size_type size) noexcept
      : _data{reinterpret_cast<const_pointer>(data)}
      , _size{size}
    {
    }

    constexpr BufferView(const const_iterator first, const const_iterator last)
      : _data{nullptr}
      , _size{0}
    {
      const difference_type dist = std::distance(first, last);
      if( dist > difference_type{0} ) {
        _data = first;
        _size = static_cast<size_type>(dist);
      }
    }

    // Iterator Support //////////////////////////////////////////////////////

    constexpr const_iterator begin() const
    {
      return _data;
    }

    constexpr const_iterator end() const
    {
      return _data + _size;
    }

    constexpr const_reverse_iterator rbegin() const
    {
      return std::make_reverse_iterator(end());
    }

    constexpr const_reverse_iterator rend() const
    {
      return std::make_reverse_iterator(begin());
    }

    // Capacity //////////////////////////////////////////////////////////////

    constexpr bool empty() const
    {
      return _size == ZERO;
    }

    constexpr size_type size() const
    {
      return _size;
    }

    // Element Access ////////////////////////////////////////////////////////

    constexpr const_reference operator[](const size_type pos) const noexcept
    {
      return _data[pos];
    }

    constexpr const_pointer data() const
    {
      return _data;
    }

    constexpr const_reference front() const
    {
      return _data[0];
    }

    constexpr const_reference back() const
    {
      return _data[_size - ONE];
    }

    // View Operations ///////////////////////////////////////////////////////

    constexpr BufferView subview(const size_type pos = 0,
                                 const size_type count = NPOS) const
    {
      return pos < _size
          ? BufferView(_data + pos, std::min(count, _size - pos))
          : BufferView();
    }

  private:
    static constexpr size_type  ONE = 1;
    static constexpr size_type ZERO = 0;

    const_pointer _data{nullptr};
    size_type     _size{0};
  };

  ////// Conversion Helpers //////////////////////////////////////////////////

  constexpr Buffer toBuffer(const BufferView& view)
  {
    return !view.empty()
        ? Buffer(view.begin(), view.end())
        : Buffer();
  }

  constexpr std::string_view toStringView(const BufferView& view)
  {
    return !view.empty()
        ? std::string_view(reinterpret_cast<const char*>(view.data()), view.size())
        : std::string_view();
  }

} // namespace cs
