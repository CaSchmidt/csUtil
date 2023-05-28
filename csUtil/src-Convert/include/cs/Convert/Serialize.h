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

#include <algorithm>

#include <cs/Core/Bit.h>
#include <cs/Core/Endian.h>
#include <cs/Core/Pointer.h>

namespace cs {

  template<typename T> requires IsIntegral<T>
  void toBytesBE(void *data, const std::size_t sizData,
                 T value)
  {
    constexpr T MASK = makeBitMask<T>(sizeof(byte_t)*8);

    if( !Pointer::isValidRange(data, sizData) ) {
      return;
    }
    byte_t *dest = reinterpret_cast<byte_t*>(data);

    const std::size_t numBytes = std::min(sizData, sizeof(T));
    if( numBytes == sizeof(T) ) {
      *reinterpret_cast<T*>(&dest[0]) = toBigEndian(value);

    } else {
      for(std::size_t i = numBytes; i > 0; /* cf. data[] */) {
        dest[--i] = byte_t(value & MASK);
        value >>= 8;
      }
    }
  }

  template<typename T> requires IsIntegral<T>
  void toBytesLE(void *data, const std::size_t sizData,
                 T value)
  {
    constexpr T MASK = makeBitMask<T>(sizeof(byte_t)*8);

    if( !Pointer::isValidRange(data, sizData) ) {
      return;
    }
    byte_t *dest = reinterpret_cast<byte_t*>(data);

    const std::size_t numBytes = std::min(sizData, sizeof(T));
    if( numBytes == sizeof(T) ) {
      *reinterpret_cast<T*>(&dest[0]) = toLittleEndian(value);

    } else {
      for(std::size_t i = 0; i < numBytes; /* cf. data[] */) {
        dest[i++] = byte_t(value & MASK);
        value >>= 8;
      }
    }
  }

} // namespace cs
