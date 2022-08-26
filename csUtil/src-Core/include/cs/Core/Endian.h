/****************************************************************************
** Copyright (c) 2016, Carsten Schmidt. All rights reserved.
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

#ifdef _MSC_VER
# include <cstdlib>
#endif

#include <bit>

#include <cs/Core/TypeTraits.h>

namespace cs {

  template<typename T>
  using is_swappable = std::bool_constant<
  is_char_v<T>      ||
  is_integral_v<T>  ||
  is_real_v<T>
  >;

  template<typename T>
  inline constexpr bool is_swappable_v = is_swappable<T>::value;

  template<typename T>
  using if_swappable_t = std::enable_if_t<std::is_swappable_v<T>,T>;

  // Implementation //////////////////////////////////////////////////////////

  namespace impl_endian {

#if defined(_MSC_VER)
    inline uint16_t impl_swap(const uint16_t& value)
    {
      return _byteswap_ushort(value);
    }

    inline uint32_t impl_swap(const uint32_t& value)
    {
      return _byteswap_ulong(value);
    }

    inline uint64_t impl_swap(const uint64_t& value)
    {
      return _byteswap_uint64(value);
    }
#elif defined(__GNUC__)
    inline uint16_t impl_swap(const uint16_t& value)
    {
      return __builtin_bswap16(value);
    }

    inline uint32_t impl_swap(const uint32_t& value)
    {
      return __builtin_bswap32(value);
    }

    inline uint64_t impl_swap(const uint64_t& value)
    {
      return __builtin_bswap64(value);
    }
#else
# error Compiler not supported!
#endif

    template<typename T>
    inline T do_swap(const T& value)
    {
      using S = typename IntegralOfSize<sizeof(T)>::unsigned_type;
      const S swapped = impl_swap(*reinterpret_cast<const S*>(&value));
      return *reinterpret_cast<const T*>(&swapped);
    }

    template<bool SWAP, typename T>
    constexpr std::enable_if_t<SWAP,T> dispatch(const T& value)
    {
      return do_swap<T>(value);
    }

    template<bool SWAP, typename T>
    constexpr std::enable_if_t<!SWAP,T> dispatch(const T& value)
    {
      return value;
    }

  } // namespace impl_endian

  // User Interface //////////////////////////////////////////////////////////

  template<bool SWAP, typename T>
  constexpr if_swappable_t<T> copy(const T& value)
  {
    return impl_endian::dispatch<SWAP  &&  sizeof(T) >= 2,T>(value);
  }

  template<typename T>
  constexpr if_swappable_t<T> swap(const T& value)
  {
    return impl_endian::dispatch<sizeof(T) >= 2,T>(value);
  }

  /*
   * Convert endianness between host byte order and 'peer' byte order.
   */

  template<typename T>
  constexpr if_swappable_t<T> fromBigEndian(const T& peerValue)
  {
    return copy<std::endian::native != std::endian::big>(peerValue);
  }

  template<typename T>
  constexpr if_swappable_t<T> fromLittleEndian(const T& peerValue)
  {
    return copy<std::endian::native != std::endian::little>(peerValue);
  }

  template<typename T>
  constexpr if_swappable_t<T> toBigEndian(const T& hostValue)
  {
    return copy<std::endian::native != std::endian::big>(hostValue);
  }

  template<typename T>
  constexpr if_swappable_t<T> toLittleEndian(const T& hostValue)
  {
    return copy<std::endian::native != std::endian::little>(hostValue);
  }

} // namespace cs
