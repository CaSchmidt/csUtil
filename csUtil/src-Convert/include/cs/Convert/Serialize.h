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
#include <cs/Core/Buffer.h>
#include <cs/Core/Container.h>
#include <cs/Core/Endian.h>
#include <cs/Core/Pointer.h>

namespace cs {

  template<typename T> requires is_arithmetic_v<T>
  void toBytesBE(void *data, const std::size_t sizData,
                 const T value)
  {
    // (1) Sanitize Destination //////////////////////////////////////////////

    if( !Pointer::isValidRange(data, sizData) ) {
      return;
    }
    byte_t *dest = reinterpret_cast<byte_t*>(data);

    // (2) Conversion ////////////////////////////////////////////////////////

    const std::size_t numBytes = std::min(sizData, sizeof(T));
    if( numBytes == sizeof(T) ) {
      *reinterpret_cast<T*>(&dest[0]) = toBigEndian(value);

    } else {
      using ConvT = typename IntegralOfSize<sizeof(T)>::unsigned_type;

      constexpr ConvT MASK = makeBitMask<ConvT>(sizeof(byte_t)*8);

      ConvT conv = *reinterpret_cast<const ConvT*>(&value);
      for(std::size_t i = numBytes; i > 0; /* cf. data[] */) {
        dest[--i] = static_cast<byte_t>(conv & MASK);
        conv >>= 8;
      }
    }
  }

  template<typename T>
  inline Buffer toBytesBE(const T value, const std::size_t count = sizeof(T))
  {
    Buffer result;
    if( !resize(result, count, 0) ) {
      return Buffer();
    }
    toBytesBE(result.data(), result.size(), value);
    return result;
  }

  template<typename T> requires is_arithmetic_v<T>
  void toBytesLE(void *data, const std::size_t sizData,
                 const T value)
  {
    // (1) Sanitize Destination //////////////////////////////////////////////

    if( !Pointer::isValidRange(data, sizData) ) {
      return;
    }
    byte_t *dest = reinterpret_cast<byte_t*>(data);

    // (2) Conversion ////////////////////////////////////////////////////////

    const std::size_t numBytes = std::min(sizData, sizeof(T));
    if( numBytes == sizeof(T) ) {
      *reinterpret_cast<T*>(&dest[0]) = toLittleEndian(value);

    } else {
      using ConvT = typename IntegralOfSize<sizeof(T)>::unsigned_type;

      constexpr ConvT MASK = makeBitMask<ConvT>(sizeof(byte_t)*8);

      ConvT conv = *reinterpret_cast<const ConvT*>(&value);
      for(std::size_t i = 0; i < numBytes; /* cf. data[] */) {
        dest[i++] = byte_t(conv & MASK);
        conv >>= 8;
      }
    }
  }

  template<typename T>
  inline Buffer toBytesLE(const T value, const std::size_t count = sizeof(T))
  {
    Buffer result;
    if( !resize(result, count, 0) ) {
      return Buffer();
    }
    toBytesLE(result.data(), result.size(), value);
    return result;
  }

} // namespace cs
