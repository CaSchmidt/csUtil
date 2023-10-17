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

    ////// Elementary Functions //////////////////////////////////////////////

    template<typename S> requires is_simd128x4f_v<S>
    inline typename S::block_type zero_w(const typename S::block_type& x)
    {
      return S::template shiftr8<4>(S::template shiftl8<4>(x));
    }

    ////// Relations /////////////////////////////////////////////////////////

    namespace impl_vec4f {

      template<bool CMP_W>
      constexpr int CMP_MASK()
      {
        return CMP_W
            ? (1 << 4) - 1
            : (1 << 3) - 1;
      }

    } // namespace impl_vec4f

    template<typename S, bool CMP_W = true> requires is_simd128x4f_v<S>
    inline bool compareLEQ(const typename S::block_type& a,
                           const typename S::block_type& b)
    {
      constexpr int MASK = impl_vec4f::CMP_MASK<CMP_W>();
      return (S::cmp_mask(S::cmp_leq(a, b)) & MASK) == MASK;
    }

    ////// 3x1 Vector Functions //////////////////////////////////////////////

    /*
     *         a0   b0   a1*b2 - a2*b1
     * a X b = a1 X b1 = a2*b0 - a0*b2
     *         a2   b2   a0*b1 - a1*b0
     */
    template<typename S> requires is_simd128x4f_v<S>
    inline typename S::block_type cross(const typename S::block_type& a,
                                        const typename S::block_type& b)
    {
      using block_t = typename S::block_type;

      const block_t prod1 = S::mul(S::template swizzle<1,2,0,3>(a),
                                   S::template swizzle<2,0,1,3>(b));
      const block_t prod2 = S::mul(S::template swizzle<2,0,1,3>(a),
                                   S::template swizzle<1,2,0,3>(b));
      return S::sub(prod1, prod2);
    }

    template<typename S> requires is_simd128x4f_v<S>
    inline typename S::block_type dot3(const typename S::block_type& a,
                                       const typename S::block_type& b)
    {
      return S::hadd(zero_w<S>(S::mul(a, b)));
    }

    ////// 4x1 Vector Functions //////////////////////////////////////////////

    template<typename S, bool CMP_W = true> requires is_simd128x4f_v<S>
    inline bool isZero(const typename S::block_type& x,
                       const typename S::block_type& epsilon0)
    {
      return compareLEQ<S,CMP_W>(S::abs(x), epsilon0);
    }

    template<typename S, bool CMP_W = true> requires is_simd128x4f_v<S>
    inline bool isZero(const typename S::block_type& x,
                       const typename S::value_type epsilon0)
    {
      return isZero<S,CMP_W>(x, S::set(epsilon0));
    }

  } // namespace simd

} // namespace cs
