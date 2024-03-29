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

#pragma once

#include <cs/Core/TypeTraits.h>
#include <cs/SIMD/SIMD128.h>

namespace n4 {

  ////// Types ///////////////////////////////////////////////////////////////

  using SIMD128 = cs::SIMD128<cs::real32_t>;

  using block_t = SIMD128::block_type;
  using  real_t = SIMD128::value_type;
  using  size_t = SIMD128::size_type;

  ////// Constants ///////////////////////////////////////////////////////////

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

  ////// Vector4f's Data Traits //////////////////////////////////////////////

  namespace impl_traits {

    template<typename DataArrayT>
    using is_data_array = std::bool_constant<
    std::is_array_v<DataArrayT>     &&
    std::rank_v<DataArrayT> == 1    &&
    std::extent_v<DataArrayT> == 4  &&
    cs::is_real32_v<std::remove_all_extents_t<DataArrayT>>
    >;

  } // namespace impl_traits

  template<typename DataT>
  using is_vector4fdata = std::bool_constant<
  impl_traits::is_data_array<decltype(std::declval<DataT>()._data)>::value
  >;

  template<typename T>
  inline constexpr bool is_vector4fdata_v = is_vector4fdata<T>::value;

} // namespace n4
