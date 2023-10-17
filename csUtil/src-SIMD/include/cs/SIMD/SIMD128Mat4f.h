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

    ////// 4x4 Matrix Functions //////////////////////////////////////////////

    namespace impl_mat4f {

      /*
       * Compute A * B
       */
      template<typename S> requires is_simd128x4f_v<S>
      inline typename S::block_type mul2x2(const typename S::block_type& a,
                                           const typename S::block_type& b)
      {
        return S::add(S::mul(                             a,
                             S::template swizzle<0,3,0,3>(b)),
                      S::mul(S::template swizzle<1,0,3,2>(a),
                             S::template swizzle<2,1,2,1>(b)));
      }

      /*
       * Compute A# * B
       */
      template<typename S> requires is_simd128x4f_v<S>
      inline typename S::block_type adjMul2x2(const typename S::block_type& a,
                                              const typename S::block_type& b)
      {
        return S::sub(S::mul(S::template swizzle<3,3,0,0>(a),
                                                          b),
                      S::mul(S::template swizzle<1,1,2,2>(a),
                             S::template swizzle<2,3,0,1>(b)));
      }

      /*
       * Compute A * B#
       */
      template<typename S> requires is_simd128x4f_v<S>
      inline typename S::block_type mulAdj2x2(const typename S::block_type& a,
                                              const typename S::block_type& b)
      {
        return S::sub(S::mul(                             a,
                             S::template swizzle<3,0,3,0>(b)),
                      S::mul(S::template swizzle<1,0,3,2>(a),
                             S::template swizzle<2,1,2,1>(b)));
      }

      /*
       * Compute the trace(A * B)
       */
      template<typename S> requires is_simd128x4f_v<S>
      inline typename S::block_type traceMul2x2(const typename S::block_type& a,
                                                const typename S::block_type& b)
      {
        return S::hadd(S::mul(a, S::template swizzle<0,2,1,3>(b)));
      }

    } // namespace impl_mat4f

    /*
     * Inverse of Column-Major 4x4 Matrix.
     *
     *               [ a0 a1 b0 b1 ]   [ 0  4  8 12 ]
     * M = [ A B ] = [ a2 a3 b2 b3 ] = [ 1  5  9 13 ]
     *     [ C D ]   [ c0 c1 d0 d1 ]   [ 2  6 10 14 ]
     *               [ c2 c3 d2 d3 ]   [ 3  7 11 15 ]
     *
     * Reference:
     * "Fast 4x4 Matrix Inverse with SSE SIMD", Eric Zhang, cf.
     * https://lxjk.github.io/2017/09/03/Fast-4x4-Matrix-Inverse-with-SSE-SIMD-Explained.html
     *
     * Assumptions/Limits of the derivation:
     * - A and D are both invertible
     * - C and D commute; AKA C*D = D*C
     */
    template<typename S> requires is_simd128x4f_v<S>
    inline void inverse(typename S::value_type *dest,
                        const typename S::value_type *src)
    {
      using block_t = typename S::block_type;

      const block_t col0 = S::load(src +  0);
      const block_t col1 = S::load(src +  4);
      const block_t col2 = S::load(src +  8);
      const block_t col3 = S::load(src + 12);

      const block_t detACBD = S::sub(
            S::mul(S::template shuffle<0,2,0,2>(col0, col2),
                   S::template shuffle<1,3,1,3>(col1, col3)),
            S::mul(S::template shuffle<1,3,1,3>(col0, col2),
                   S::template shuffle<0,2,0,2>(col1, col3))
            );

      const block_t detA = S::template swizzle<0,0,0,0>(detACBD);
      const block_t detB = S::template swizzle<2,2,2,2>(detACBD);
      const block_t detC = S::template swizzle<1,1,1,1>(detACBD);
      const block_t detD = S::template swizzle<3,3,3,3>(detACBD);

      const block_t A = S::intrlvlo(col0, col1);
      const block_t B = S::intrlvlo(col2, col3);
      const block_t C = S::intrlvhi(col0, col1);
      const block_t D = S::intrlvhi(col2, col3);

      const block_t AadjB = impl_mat4f::adjMul2x2<S>(A, B);
      const block_t DadjC = impl_mat4f::adjMul2x2<S>(D, C);

      // NOTE: Combine adjugate's signs divided by detM.
      const block_t detM = S::div(S::set(1, -1, -1, 1),
                                  S::sub(S::add(S::mul(detA, detD), S::mul(detB, detC)),
                                         impl_mat4f::traceMul2x2<S>(AadjB, DadjC)));

      const block_t X = S::mul(detM, S::sub(S::mul(detD, A),
                                            impl_mat4f::mul2x2<S>(B, DadjC)));
      const block_t Y = S::mul(detM, S::sub(S::mul(detB, C),
                                            impl_mat4f::mulAdj2x2<S>(D, AadjB)));
      const block_t Z = S::mul(detM, S::sub(S::mul(detC, B),
                                            impl_mat4f::mulAdj2x2<S>(A, DadjC)));
      const block_t W = S::mul(detM, S::sub(S::mul(detA, D),
                                            impl_mat4f::mul2x2<S>(C, AadjB)));

      /*
       * NOTE: Account for the adjugate swapping elements along the main diagonal!
       *        [ x3 x1 y3 y1 ]
       * Minv = [ x2 x0 y2 y0 ]
       *        [ z3 z1 w3 w1 ]
       *        [ z2 z0 w2 w0 ]
       */
      S::store(dest +  0, S::template shuffle<3,2,3,2>(X, Z));
      S::store(dest +  4, S::template shuffle<1,0,1,0>(X, Z));
      S::store(dest +  8, S::template shuffle<3,2,3,2>(Y, W));
      S::store(dest + 12, S::template shuffle<1,0,1,0>(Y, W));
    }

    /*
     * Linear Transformation of 4x1 Column Vector by 4x4 Column-Major Matrix.
     */
    template<typename S> requires is_simd128x4f_v<S>
    inline typename S::block_type transform(const typename S::block_type& col0,
                                            const typename S::block_type& col1,
                                            const typename S::block_type& col2,
                                            const typename S::block_type& col3,
                                            const typename S::block_type& x)
    {
      using block_t = typename S::block_type;

      // NOTE: y = M*x
      block_t   y = S::mul(S::template swizzle<0,0,0,0>(x), col0);
      y = S::add(y, S::mul(S::template swizzle<1,1,1,1>(x), col1));
      y = S::add(y, S::mul(S::template swizzle<2,2,2,2>(x), col2));
      y = S::add(y, S::mul(S::template swizzle<3,3,3,3>(x), col3));
      return y;
    }

    /*
     * Transpose of Column-Major 4x4 Matrix.
     */
    template<typename S> requires is_simd128x4f_v<S>
    inline void transpose(typename S::value_type *dest,
                          const typename S::value_type *src)
    {
      using block_t = typename S::block_type;

      block_t col0 = S::load(src +  0);
      block_t col1 = S::load(src +  4);
      block_t col2 = S::load(src +  8);
      block_t col3 = S::load(src + 12);

      const block_t tmp0 = S::intrlvlo(col0, col2);
      const block_t tmp1 = S::intrlvhi(col0, col2);
      const block_t tmp2 = S::intrlvlo(col1, col3);
      const block_t tmp3 = S::intrlvhi(col1, col3);
      col0 = S::intrlvlo(tmp0, tmp2);
      col1 = S::intrlvhi(tmp0, tmp2);
      col2 = S::intrlvlo(tmp1, tmp3);
      col3 = S::intrlvhi(tmp1, tmp3);

      S::store(dest +  0, col0);
      S::store(dest +  4, col1);
      S::store(dest +  8, col2);
      S::store(dest + 12, col3);
    }

  } // namespace simd

} // namespace cs
