/****************************************************************************
** Copyright (c) 2018, Carsten Schmidt. All rights reserved.
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

#include <cstddef>
#include <cstdint>

#include <limits>
#include <type_traits>

namespace cs {

  ////// Portable Types //////////////////////////////////////////////////////

  using byte_t = uint8_t;

  using real32_t = float;
  using real64_t = double;

  using size_t = std::size_t;

  static_assert( std::is_unsigned_v<size_t> );

  ////// Type Categories /////////////////////////////////////////////////////

  // Boolean Category ////////////////////////////////////////////////////////

  template<typename T>
  using is_boolean = std::bool_constant<
  std::is_same_v<T,bool>
  >;

  template<typename T>
  inline constexpr bool is_boolean_v = is_boolean<T>::value;

  template<typename T, typename ResultT = T>
  using if_boolean_t = std::enable_if_t<is_boolean_v<T>,ResultT>;

  // Integral Category ///////////////////////////////////////////////////////

  template<typename T>
  using is_signed = std::bool_constant<
  std::is_same_v<T,int8_t>   ||
  std::is_same_v<T,int16_t>  ||
  std::is_same_v<T,int32_t>  ||
  std::is_same_v<T,int64_t>  ||
  std::is_same_v<T,long>
  >;

  template<typename T>
  inline constexpr bool is_signed_v = is_signed<T>::value;

  template<typename T, typename ResultT = T>
  using if_signed_t = std::enable_if_t<is_signed_v<T>,ResultT>;

  template<typename T>
  using is_unsigned = std::bool_constant<
  std::is_same_v<T,uint8_t>        ||
  std::is_same_v<T,uint16_t>       ||
  std::is_same_v<T,uint32_t>       ||
  std::is_same_v<T,uint64_t>       ||
  std::is_same_v<T,unsigned long>
  >;

  template<typename T>
  inline constexpr bool is_unsigned_v = is_unsigned<T>::value;

  template<typename T, typename ResultT = T>
  using if_unsigned_t = std::enable_if_t<is_unsigned_v<T>,ResultT>;

  template<typename T>
  using is_integral = std::bool_constant<
  is_signed_v<T>    ||
  is_unsigned_v<T>
  >;

  template<typename T>
  inline constexpr bool is_integral_v = is_integral<T>::value;

  template<typename T, typename ResultT = T>
  using if_integral_t = std::enable_if_t<is_integral_v<T>,ResultT>;

  // Real Category ///////////////////////////////////////////////////////////

  template<typename T>
  using is_real = std::bool_constant<
  std::is_same_v<T,real32_t>  ||
  std::is_same_v<T,real64_t>
  >;

  template<typename T>
  inline constexpr bool is_real_v = is_real<T>::value;

  template<typename T, typename ResultT = T>
  using if_real_t = std::enable_if_t<is_real_v<T>,ResultT>;

  template<typename T>
  using is_real32 = std::bool_constant<
  is_real_v<T>  &&  sizeof(T) == 4
  >;

  template<typename T>
  inline constexpr bool is_real32_v = is_real32<T>::value;

  template<typename T>
  using is_real64 = std::bool_constant<
  is_real_v<T>  &&  sizeof(T) == 8
  >;

  template<typename T>
  inline constexpr bool is_real64_v = is_real64<T>::value;

  // Arithmetic Category /////////////////////////////////////////////////////

  template<typename T>
  using is_arithmetic = std::bool_constant<
  is_integral_v<T>  ||
  is_real_v<T>
  >;

  template<typename T>
  inline constexpr bool is_arithmetic_v = is_arithmetic<T>::value;

  // Character Category //////////////////////////////////////////////////////

  template<typename T>
  using is_char = std::bool_constant<
  std::is_same_v<T,char>      ||
  std::is_same_v<T,char8_t>   ||
  std::is_same_v<T,char16_t>  ||
  std::is_same_v<T,char32_t>  ||
  std::is_same_v<T,wchar_t>
  >;

  template<typename T>
  inline constexpr bool is_char_v = is_char<T>::value;

  template<typename T>
  using is_narrowchar = std::bool_constant<is_char_v<T>  &&  sizeof(T) == 1>;

  template<typename T>
  inline constexpr bool is_narrowchar_v = is_narrowchar<T>::value;

  template<typename T>
  using is_widechar = std::bool_constant<is_char_v<T>  &&  (sizeof(T) > 1)>;

  template<typename T>
  inline constexpr bool is_widechar_v = is_widechar<T>::value;

  // Generic Category ////////////////////////////////////////////////////////

  /*
   * NOTE:
   *
   * "How to check if a number is a power of 2 in C++", Approach 2, cf.
   * https://www.educative.io/answers/how-to-check-if-a-number-is-a-power-of-2-in-cpp
   */
  template<typename T>
  using is_pow2size = std::bool_constant<
  sizeof(T) != 0  &&  (sizeof(T) & (sizeof(T) - 1)) == 0
  >;

  template<typename T>
  inline constexpr bool is_pow2size_v = is_pow2size<T>::value;

  ////// Integral Types //////////////////////////////////////////////////////

  template<std::size_t SIZE>
  struct IntegralOfSize {
    // SFINAE
  };

  template<>
  struct IntegralOfSize<1> {
    using   signed_type =  int8_t;
    using unsigned_type = uint8_t;
  };

  template<>
  struct IntegralOfSize<2> {
    using   signed_type =  int16_t;
    using unsigned_type = uint16_t;
  };

  template<>
  struct IntegralOfSize<4> {
    using   signed_type =  int32_t;
    using unsigned_type = uint32_t;
  };

  template<>
  struct IntegralOfSize<8> {
    using   signed_type =  int64_t;
    using unsigned_type = uint64_t;
  };

  ////// Real Types //////////////////////////////////////////////////////////

  template<std::size_t SIZE>
  struct RealOfSize {
    // SFINAE
  };

  template<>
  struct RealOfSize<4> {
    using real_type = real32_t;
  };

  template<>
  struct RealOfSize<8> {
    using real_type = real64_t;
  };

  ////// Integral Transformations ////////////////////////////////////////////

  template<typename T>
  using make_signed_t =
  std::enable_if_t<is_integral_v<T>,typename IntegralOfSize<sizeof(T)>::signed_type>;

  template<typename T>
  constexpr make_signed_t<T> signed_cast(const T value)
  {
    return static_cast<make_signed_t<T>>(value);
  }

  template<typename T>
  using make_unsigned_t =
  std::enable_if_t<is_integral_v<T>,typename IntegralOfSize<sizeof(T)>::unsigned_type>;

  template<typename T>
  constexpr make_unsigned_t<T> unsigned_cast(const T value)
  {
    return static_cast<make_unsigned_t<T>>(value);
  }

  ////// Miscellaneous Transformations ///////////////////////////////////////

  template<typename T>
  using safe_underlying_type_t =
  std::enable_if_t<std::is_enum_v<T>,std::underlying_type_t<T>>;

  ////// Constants ///////////////////////////////////////////////////////////

  inline constexpr auto MAX_SIZE_T = std::numeric_limits<std::size_t>::max();

  template<typename A, typename B>
  using maxab = std::integral_constant<A,
  sizeof(A) == sizeof(B)
  ? std::is_signed_v<A>
    ? std::numeric_limits<A>::max()
    // NOTE: Handle both cases where B is signed and both A and B are unsigned!
    : std::numeric_limits<B>::max()
  : (sizeof(A) > sizeof(B))
    ? std::numeric_limits<B>::max()
    // NOTE: sizeof(A) < sizeof(B)
    : std::numeric_limits<A>::max()
  >;

  template<typename A, typename B>
  inline constexpr A maxab_v = maxab<A,B>::value;

  ////// Same Types //////////////////////////////////////////////////////////

  template<typename T, typename REF>
  using if_same_t = std::enable_if_t<std::is_same_v<T,REF>,T>;

  ////// Pointer Conversions /////////////////////////////////////////////////

  inline const char *CSTR(const char8_t *s)
  {
    return reinterpret_cast<const char*>(s);
  }

  inline const wchar_t *WSTR(const char16_t *s,
                             std::enable_if_t<sizeof(wchar_t) == sizeof(char16_t)> * = nullptr)
  {
    return reinterpret_cast<const wchar_t*>(s);
  }

  inline const char8_t *UTF8(const char *s)
  {
    return reinterpret_cast<const char8_t*>(s);
  }

  inline const char16_t *UTF16(const wchar_t *s,
                               std::enable_if_t<sizeof(char16_t) == sizeof(wchar_t)> * = nullptr)
  {
    return reinterpret_cast<const char16_t*>(s);
  }

} // namespace cs
