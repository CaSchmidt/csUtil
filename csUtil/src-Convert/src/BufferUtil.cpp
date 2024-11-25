/****************************************************************************
** Copyright (c) 2023, Carsten Schmidt. All rights reserved.
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

#include "cs/Convert/BufferUtil.h"

#include "cs/Core/CharUtil.h"
#include "cs/Core/Constants.h"
#include "cs/Core/Container.h"
#include "cs/Math/Numeric.h"

namespace cs {

  ////// Private /////////////////////////////////////////////////////////////

  namespace impl_bytearray {

    template<bool UPPER>
    inline void toHexString(std::string::iterator d_first,
                            const BufferView& data, const char sep)
    {
      for(const Buffer::value_type b : data) {
        *d_first = toHexChar<char,UPPER>(b, true);
        ++d_first;

        *d_first = toHexChar<char,UPPER>(b);
        ++d_first;

        if( sep != '\0' ) {
          ++d_first;
        }
      } // For Each Byte
    }

  } // namespace impl_bytearray

  ////// Public //////////////////////////////////////////////////////////////

  CS_UTIL_EXPORT auto fromHexString(const std::string_view& str)
  -> std::expected<Buffer,std::errc>
  {
    using difference_type = std::string_view::difference_type;

    using k = Konst<std::size_t>;

    // (0) Sanitize Input ////////////////////////////////////////////////////

    if( str.empty()  ||  isOdd(str.size()) ) {
      return std::unexpected(std::errc::invalid_argument);
    }

    const std::size_t numHexChars = std::max(difference_type{0},
                                             std::count_if(str.begin(), str.end(),
                                                           lambda_is_hex<char>()));
    if( numHexChars != str.size() ) {
      return std::unexpected(std::errc::invalid_argument);
    }

    // (1) Create Result /////////////////////////////////////////////////////

    Buffer result;
    if( !resize(result, numHexChars/k::TWO) ) {
      return std::unexpected(std::errc::not_enough_memory);
    }

    // (2) Conversion ////////////////////////////////////////////////////////

    auto src = str.begin();
    for(byte_t& b : result) {
      b  = fromHexChar(*src) << 4;
      ++src;

      b |= fromHexChar(*src);
      ++src;
    }

    // Done! /////////////////////////////////////////////////////////////////

    return result;
  }

  CS_UTIL_EXPORT auto toHexString(const BufferView& data,
                                  const char sep,
                                  const bool is_upper)
  -> std::expected<std::string,std::errc>
  {
    using k = Konst<std::size_t>;

    // (0) Sanitize Input ////////////////////////////////////////////////////

    if( data.empty() ) {
      return std::unexpected(std::errc::invalid_argument);
    }

    // (1) Create Result /////////////////////////////////////////////////////

    // Reserve output length, including separator.
    const std::size_t length = sep != '\0'
        ? data.size()*k::THREE - k::ONE // sizeData*2 + (sizeData - 1)
        : data.size()*k::TWO;

    std::string result;
    if( !resize(result, length, sep) ) {
      return std::unexpected(std::errc::not_enough_memory);
    }

    // (2) Conversion ////////////////////////////////////////////////////////

    if( is_upper ) {
      impl_bytearray::toHexString<true>(result.begin(), data, sep);
    } else {
      impl_bytearray::toHexString<false>(result.begin(), data, sep);
    }

    // Done! /////////////////////////////////////////////////////////////////

    return result;
  }

} // namespace cs
