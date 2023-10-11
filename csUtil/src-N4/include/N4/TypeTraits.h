/****************************************************************************
** Copyright (c) 2020, Carsten Schmidt. All rights reserved.
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

#ifndef N4_TYPETRAITS_H
#define N4_TYPETRAITS_H

#include <cstddef>
#include <cstdint>

#include <type_traits>

namespace n4 {

  ////// Type Traits /////////////////////////////////////////////////////////

  template<typename T>
  using is_real = std::bool_constant<std::is_floating_point_v<T>  &&  sizeof(T) == 4>;

  template<typename T>
  using if_real_t = std::enable_if_t<is_real<T>::value,T>;

  ////// Types ///////////////////////////////////////////////////////////////

  using real_t = if_real_t<float>;

  using std::size_t;

  ////// Constants ///////////////////////////////////////////////////////////

  inline constexpr real_t EPSILON0_PCT    = 0x1p-7;  // ca. 0.01
  inline constexpr real_t EPSILON0_SCALAR = 0x1p-20; // ca. 0.000001
  inline constexpr real_t EPSILON0_VECTOR = 0x1p-13; // ca. 0.0001

  ////// Assign W Component //////////////////////////////////////////////////

  template<typename T, typename = bool>
  struct is_assign_w : std::false_type {};

  template<typename T>
  struct is_assign_w<T,decltype((void)T::have_assign_w,bool())> : std::true_type {};

  template<typename T>
  inline constexpr bool is_assign_w_v = is_assign_w<T>::value;

  template<typename T>
  constexpr std::enable_if_t<is_assign_w_v<T>,bool> have_assign_w()
  {
    return static_cast<bool>(T::have_assign_w);
  }

  template<typename T>
  constexpr std::enable_if_t<!is_assign_w_v<T>,bool> have_assign_w()
  {
    return false;
  }

} // namespace n4

#endif // N4_TYPETRAITS_H
