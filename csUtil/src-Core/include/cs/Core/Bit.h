/****************************************************************************
** Copyright (c) 2022, Carsten Schmidt. All rights reserved.
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

#include <cs/Core/Constants.h>

namespace cs {

  template<typename T> requires IsIntegral<T>
  inline constexpr std::size_t MAX_BITS = sizeof(T)*8;

  template<typename T> requires IsIntegral<T>
  inline constexpr std::size_t MAX_BIT = MAX_BITS<T> - 1;

  namespace impl_bit {

    template<typename T> requires IsIntegral<T>
    constexpr bool testBit(const T in, const std::size_t bit)
    {
      using k = konst<T>;
      return (in & (k::ONE << bit)) != k::ZERO;
    }

    template<typename T> requires IsIntegral<T>
    constexpr T reflectBit(const T in, const std::size_t bit)
    {
      using k = konst<T>;
      return testBit(in, bit)
          ? k::ONE << (MAX_BIT<T> - bit)
          : k::ZERO;
    }

    template<typename T> requires IsIntegral<T>
    constexpr T makeBitMaskImpl(const std::size_t bit)
    {
      using k = konst<T>;
      return bit > std::size_t(0)
          ? (k::ONE << bit) | makeBitMaskImpl<T>(bit - 1)
          : k::ONE;
    }

    template<typename T> requires IsIntegral<T>
    constexpr T reverseBitsImpl(const T in, const std::size_t bit)
    {
      return bit > std::size_t(0)
          ? reflectBit(in, bit) | reverseBitsImpl(in, bit - 1)
          : reflectBit(in, bit);
    }

  } // namespace impl_bit

  template<typename T> requires IsIntegral<T>
  constexpr T makeBitMask(const std::size_t bits)
  {
    return std::size_t(0) < bits  &&  bits <= MAX_BITS<T>
        ? impl_bit::makeBitMaskImpl<T>(bits - 1)
        : konst<T>::ZERO;
  }

  template<typename T> requires IsIntegral<T>
  constexpr T reverseBits(const T in)
  {
    return impl_bit::reverseBitsImpl(in, MAX_BIT<T>);
  }

  ////// Meta Programming based Implementation ///////////////////////////////

  namespace impl_bit {

    template<typename T, std::size_t BIT> requires IsIntegral<T>
    struct BitMaskImpl {
      static_assert( 0 < BIT  &&  BIT < sizeof(T)*8 );

      static constexpr T run()
      {
        return (konst<T>::ONE << BIT) | BitMaskImpl<T,BIT-1>::run();
      }
    };

    template<typename T> requires IsIntegral<T>
    struct BitMaskImpl<T,0> {
      static constexpr T run()
      {
        return konst<T>::ONE;
      }
    };

  } // namespace impl_bit

  template<typename T, std::size_t BITS> requires IsIntegral<T>
  constexpr T makeBitMask()
  {
    return impl_bit::BitMaskImpl<T,BITS-1>::run();
  }

} // namespace cs
