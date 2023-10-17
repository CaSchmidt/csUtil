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

#include <cs/Core/TypeTraits.h>

namespace cs {

  namespace simd {

    // SIMD Width ////////////////////////////////////////////////////////////

    template<typename SIMD>
    using is_simd128 = std::bool_constant<
    sizeof(typename SIMD::block_type) == 16
    >;

    template<typename SIMD>
    inline constexpr bool is_simd128_v = is_simd128<SIMD>::value;

    // Integral Type /////////////////////////////////////////////////////////

    template<typename SIMD>
    using is_simd_integral = std::bool_constant<
    is_integral_v<typename SIMD::value_type>
    >;

    template<typename SIMD>
    inline constexpr bool is_simd_integral_v = is_simd_integral<SIMD>::value;

    // Real Type /////////////////////////////////////////////////////////////

    template<typename SIMD>
    using is_simd_real = std::bool_constant<
    is_real_v<typename SIMD::value_type>
    >;

    template<typename SIMD>
    inline constexpr bool is_simd_real_v = is_simd_real<SIMD>::value;

    // Number of Elements ////////////////////////////////////////////////////

    template<typename SIMD, std::size_t NUM_ELEM>
    using is_simd_elem = std::bool_constant<
    sizeof(typename SIMD::block_type)/sizeof(typename SIMD::value_type) == NUM_ELEM
    >;

    template<typename SIMD, std::size_t NUM_ELEM>
    inline constexpr bool is_simd_elem_v = is_simd_elem<SIMD,NUM_ELEM>::value;

    // SIMD128 4x 32bit Real /////////////////////////////////////////////////

    template<typename SIMD>
    using is_simd128x4f = std::bool_constant<
    is_real32_v<typename SIMD::value_type>  &&  is_simd_elem_v<SIMD,4>
    >;

    template<typename SIMD>
    inline constexpr bool is_simd128x4f_v = is_simd128x4f<SIMD>::value;

  } // namespace simd

} // namespace cs
