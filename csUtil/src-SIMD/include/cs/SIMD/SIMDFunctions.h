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

    template<typename S>
    inline typename S::block_type hadd(const typename S::block_type& x,
                                       const std::enable_if_t<is_simd_elem_v<S,2>> * = nullptr)
    {
      return S::add(x, S::template swizzle<1,0>(x));
    }

    template<typename S>
    inline typename S::block_type hadd(const typename S::block_type& x,
                                       const std::enable_if_t<is_simd_elem_v<S,4>> * = nullptr)
    {
      const typename S::block_type y = S::add(x, S::template swizzle<1,0,3,2>(x));
      return                           S::add(y, S::template swizzle<3,2,1,0>(y));
    }

    // Bit Operations ////////////////////////////////////////////////////////

    template<typename S>
    inline typename S::block_type one()
    {
      return S::cmp_eq(S::zero(), S::zero());
    }

    // Math //////////////////////////////////////////////////////////////////

    template<typename S>
    inline typename S::block_type abs(const typename S::block_type& x,
                                      const std::enable_if_t<is_simd_integral_v<S>> * = nullptr)
    {
      const typename S::block_type cmpl2 = // Two's Complement
          S::add(S::bit_xor(x, S::one()), S::template shiftr<31>(S::one()));
      return S::cmov(S::cmp_gt(cmpl2, x), cmpl2, x);
    }

    template<typename S>
    inline typename S::block_type abs(const typename S::block_type& x,
                                      const std::enable_if_t<is_simd_real_v<S>> * = nullptr)
    {
      return S::bit_and(x, S::template shiftr<1>(S::one()));
    }

    // Relations /////////////////////////////////////////////////////////////

    template<typename S>
    inline typename S::block_type clamp(const typename S::block_type& x,
                                        const typename S::block_type& lo,
                                        const typename S::block_type& hi)
    {
      return S::max(lo, S::min(x, hi));
    }

    template<typename S>
    inline typename S::block_type cmov(const typename S::block_type& cond_a,
                                       const typename S::block_type& a,
                                       const typename S::block_type& b)
    {
      return S::bit_or(S::bit_and(cond_a, a), S::bit_andnot(cond_a, b));
    }

  } // namespace simd

} // namespace cs
