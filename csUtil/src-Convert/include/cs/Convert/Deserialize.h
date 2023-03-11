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

#pragma once

#include <cs/Core/Endian.h>
#include <cs/Core/Range.h>

namespace cs {

  template<typename T> requires IsIntegral<T>
  inline T toIntegralFromBE(const uint8_t *data, const std::size_t sizData)
  {
    if( !isValid(data, sizData) ) {
      return T{0};
    }

    T value{0};
    const std::size_t numBytes = std::min(sizData, sizeof(T));
    if( numBytes == sizeof(T) ) {
      value = fromBigEndian(*reinterpret_cast<const T*>(&data[0]));
    } else {
      for(std::size_t i = 0; i < numBytes; /* cf. data[] */) {
        value <<= 8;
        value  |= T(data[i++]);
      }
    }

    return value;
  }

  template<typename T> requires IsIntegral<T>
  inline T toIntegralFromLE(const uint8_t *data, const std::size_t sizData)
  {
    if( !isValid(data, sizData) ) {
      return T{0};
    }

    T value{0};
    const std::size_t numBytes = std::min(sizData, sizeof(T));
    if( numBytes == sizeof(T) ) {
      value = fromLittleEndian(*reinterpret_cast<const T*>(&data[0]));
    } else {
      for(std::size_t i = numBytes; i > 0; /* cf. data[] */) {
        value <<= 8;
        value  |= T(data[--i]);
      }
    }

    return value;
  }

} // namespace cs
