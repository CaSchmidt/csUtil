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

#ifndef CSENDIAN_H
#define CSENDIAN_H

#ifdef _MSC_VER
# include <cstdlib>
#endif
#include <cstdint>

#include <type_traits>

namespace cs {

  namespace impl {

#if defined(_MSC_VER)
    inline uint16_t impl_swap16(const uint16_t& value)
    {
      return _byteswap_ushort(value);
    }

    inline uint32_t impl_swap32(const uint32_t& value)
    {
      return _byteswap_ulong(value);
    }

    inline uint64_t impl_swap64(const uint64_t& value)
    {
      return _byteswap_uint64(value);
    }
#elif defined(__GNUC__)
    inline uint16_t impl_swap16(const uint16_t& value)
    {
      return __builtin_bswap16(value);
    }

    inline uint32_t impl_swap32(const uint32_t& value)
    {
      return __builtin_bswap32(value);
    }

    inline uint64_t impl_swap64(const uint64_t& value)
    {
      return __builtin_bswap64(value);
    }
#else
# error Compiler not supported!
#endif

    template<typename T>
    inline std::enable_if_t<sizeof(T) == 2,T> swap(const T& value)
    {
      const uint16_t swapped = impl_swap16(*reinterpret_cast<const uint16_t*>(&value));
      return *reinterpret_cast<const T*>(&swapped);
    }

    template<typename T>
    inline std::enable_if_t<sizeof(T) == 4,T> swap(const T& value)
    {
      const uint32_t swapped = impl_swap32(*reinterpret_cast<const uint32_t*>(&value));
      return *reinterpret_cast<const T*>(&swapped);
    }

    template<typename T>
    inline std::enable_if_t<sizeof(T) == 8,T> swap(const T& value)
    {
      const uint64_t swapped = impl_swap64(*reinterpret_cast<const uint64_t*>(&value));
      return *reinterpret_cast<const T*>(&swapped);
    }

    template<bool SWAP, typename T>
    inline std::enable_if_t<SWAP == true,T> dispatch(const T& value)
    {
      return swap<T>(value);
    }

    template<bool SWAP, typename T>
    inline std::enable_if_t<SWAP == false,T> dispatch(const T& value)
    {
      return value;
    }

  } // namespace impl

  template<bool SWAP, typename T>
  inline std::enable_if_t<std::is_arithmetic<T>::value,T> swap(const T& value)
  {
    return impl::dispatch<SWAP  &&  sizeof(T) >= 2,T>(value);
  }

} // namespace cs

#endif // CSENDIAN_H
