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

#ifndef CSTYPETRAITS_H
#define CSTYPETRAITS_H

#include <limits>
#include <type_traits>

namespace cs {

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
  using safe_underlying_type_t =
  std::enable_if_t<std::is_enum_v<T>,std::underlying_type_t<T>>;

  inline constexpr auto MAX_SIZE_T = std::numeric_limits<std::size_t>::max();

  inline constexpr auto qNaN = std::numeric_limits<double>::quiet_NaN();

} // namespace cs

#endif // CSTYPETRAITS_H
