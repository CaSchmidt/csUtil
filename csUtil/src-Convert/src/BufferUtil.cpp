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
#include "cs/Text/StringAlgorithm.h"

namespace cs {

  ////// Private /////////////////////////////////////////////////////////////

  namespace impl_bytearray {

    template<bool UPPER, typename T> requires IsCharacter<T>
    inline void toString(T *dest, const byte_t *src, const std::size_t lenSrc,
                         const std::size_t dpos)
    {
      for(std::size_t i = 0, pos = 0; i < lenSrc; i++, pos += dpos) {
        dest[pos + 0] = toHexChar<T,UPPER>(src[i], true);
        dest[pos + 1] = toHexChar<T,UPPER>(src[i]);
      }
    }

  } // namespace impl_bytearray

  ////// Public //////////////////////////////////////////////////////////////

  CS_UTIL_EXPORT Buffer toBuffer(const char *str, const std::size_t lenStr)
  {
    using k = konst<std::size_t>;

    if( !isHexString(str, lenStr) ) {
      return Buffer{};
    }

    Buffer result;
    if( !resize(&result, (lenStr + k::ONE)/k::TWO) ) {
      return Buffer{};
    }

    const std::size_t i0 = result.size() != lenStr/k::TWO
        ? 1
        : 0;

    if( i0 > k::ZERO ) {
      result[0] = fromHexChar(str[0]);
    }

    for(std::size_t i = i0; i < result.size(); i++) {
      const std::size_t idxStr = i*k::TWO - i0;
      result[i]  = fromHexChar(str[idxStr + 0]) << 4;
      result[i] |= fromHexChar(str[idxStr + 1]);
    }

    return result;
  }

  CS_UTIL_EXPORT std::string toString(const void *data, const std::size_t sizData,
                                      const char fill, const bool is_upper)
  {
    using k = konst<std::size_t>;

    if( !isValid(data, sizData) ) {
      return std::string{};
    }

    const std::size_t length = fill != '\0'
        ? sizData*k::THREE - k::ONE // sizeData*2 + (sizeData - 1)
        : sizData*k::TWO;

    std::string result;
    if( !resize(&result, length, fill) ) {
      return std::string{};
    }

    const std::size_t dpos = fill != '\0'
        ? 3
        : 2;

    const byte_t *src = reinterpret_cast<const byte_t*>(data);
    if( is_upper ) {
      impl_bytearray::toString<true>(result.data(), src, sizData, dpos);
    } else {
      impl_bytearray::toString<false>(result.data(), src, sizData, dpos);
    }

    return result;
  }

} // namespace cs
