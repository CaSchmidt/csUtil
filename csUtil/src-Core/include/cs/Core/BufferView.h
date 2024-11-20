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

#include <iterator>

#include <cs/Core/TypeTraits.h>

namespace cs {

  class BufferView {
  public:
    using difference_type = std::ptrdiff_t;
    using       size_type = std::size_t;
    using      value_type = byte_t;

    using const_pointer = const value_type *;

    using const_reference = const value_type &;

    using const_iterator = const_pointer;

    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // Construction and Assignment ///////////////////////////////////////////

    constexpr BufferView() noexcept
      : _data{nullptr}
      , _size{0}
    {
    }

    BufferView(const void *data, const size_type size) noexcept
      : _data{reinterpret_cast<const_pointer>(data)}
      , _size{size}
    {
    }

    constexpr BufferView& operator=(const BufferView& other) noexcept
    {
      _data = other._data;
      _size = other._size;

      return *this;
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

    constexpr const_iterator cbegin() const
    {
      return _data;
    }

    constexpr const_iterator cend() const
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

    constexpr const_reverse_iterator crbegin() const
    {
      return std::make_reverse_iterator(cend());
    }

    constexpr const_reverse_iterator crend() const
    {
      return std::make_reverse_iterator(cbegin());
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

  private:
    static constexpr size_type  ONE = 1;
    static constexpr size_type ZERO = 0;

    const_pointer _data{nullptr};
    size_type     _size{0};
  };

} // namespace cs
