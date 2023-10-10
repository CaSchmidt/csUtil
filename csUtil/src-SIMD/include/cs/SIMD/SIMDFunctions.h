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

#include <cs/SIMD/SIMDTraits.h>

namespace cs {

  namespace simd {

    // Arithmetic ////////////////////////////////////////////////////////////

    template<typename SIMD>
    typename SIMD::block_type hadd(const typename SIMD::block_type& x,
                                   const std::enable_if_t<is_simd_elem_v<SIMD,2>> * = nullptr)
    {
      return SIMD::add(x, SIMD::template swizzle<1,0>(x));
    }

    template<typename SIMD>
    typename SIMD::block_type hadd(const typename SIMD::block_type& x,
                                   const std::enable_if_t<is_simd_elem_v<SIMD,4>> * = nullptr)
    {
      const typename SIMD::block_type y = SIMD::add(x, SIMD::template swizzle<1,0,3,2>(x));
      return                              SIMD::add(y, SIMD::template swizzle<3,2,1,0>(y));
    }

    // Bit Operations ////////////////////////////////////////////////////////

    template<typename SIMD>
    typename SIMD::block_type one()
    {
      return SIMD::cmp_eq(SIMD::zero(), SIMD::zero());
    }

    // Math //////////////////////////////////////////////////////////////////

    template<typename SIMD>
    typename SIMD::block_type abs(const typename SIMD::block_type& x,
                                  const std::enable_if_t<is_simd_integral_v<SIMD>> * = nullptr)
    {
      const typename SIMD::block_type cmpl2 = // Two's Complement
          SIMD::add(SIMD::bit_xor(x, SIMD::one()), SIMD::template shiftr<31>(SIMD::one()));
      return SIMD::cmov(SIMD::cmp_gt(cmpl2, x), cmpl2, x);
    }

    template<typename SIMD>
    typename SIMD::block_type abs(const typename SIMD::block_type& x,
                                  const std::enable_if_t<is_simd_real_v<SIMD>> * = nullptr)
    {
      return SIMD::bit_and(x, SIMD::template shiftr<1>(SIMD::one()));
    }

    // Relations /////////////////////////////////////////////////////////////

    template<typename SIMD>
    typename SIMD::block_type cmov(const typename SIMD::block_type& cond_a,
                                   const typename SIMD::block_type& a,
                                   const typename SIMD::block_type& b)
    {
      return SIMD::bit_or(SIMD::bit_and(cond_a, a), SIMD::bit_andnot(cond_a, b));
    }

  } // namespace simd

} // namespace cs
