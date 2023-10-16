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

#include <cs/SIMD/SIMD128.h>

#include <N4/N4Traits.h>

namespace n4 {

  namespace simd {

    ////// Types /////////////////////////////////////////////////////////////

    using SIMD128 = cs::SIMD128<real_t>;
    using block_t = SIMD128::block_type;

    ////// Assertions ////////////////////////////////////////////////////////

    static_assert( cs::simd::is_simd128x4f_v<SIMD128> );

    ////// Elementary Functions //////////////////////////////////////////////

    inline block_t zero_w(const block_t& x)
    {
      return SIMD128::shiftr8<4>(SIMD128::shiftl8<4>(x));
    }

    ////// Relations /////////////////////////////////////////////////////////

    namespace impl {

      template<bool CMP_W>
      constexpr int CMP_MASK()
      {
        return CMP_W
            ? (1 << 4) - 1
            : (1 << 3) - 1;
      }

    } // namespace impl

    template<bool CMP_W = true>
    inline bool compareLEQ(const block_t& a, const block_t& b)
    {
      constexpr int MASK = impl::CMP_MASK<CMP_W>();
      return (SIMD128::cmp_mask(SIMD128::cmp_leq(a, b)) & MASK) == MASK;
    }

    ////// 3x1 Vector Functions //////////////////////////////////////////////

    /*
     *         a0   b0   a1*b2 - a2*b1
     * a X b = a1 X b1 = a2*b0 - a0*b2
     *         a2   b2   a0*b1 - a1*b0
     */
    inline block_t cross(const block_t& a, const block_t& b)
    {
      const block_t prod1 = SIMD128::mul(SIMD128::swizzle<1,2,0,3>(a),
                                         SIMD128::swizzle<2,0,1,3>(b));
      const block_t prod2 = SIMD128::mul(SIMD128::swizzle<2,0,1,3>(a),
                                         SIMD128::swizzle<1,2,0,3>(b));
      return SIMD128::sub(prod1, prod2);
    }

    inline block_t dot3(const block_t& a, const block_t& b)
    {
      return SIMD128::hadd(zero_w(SIMD128::mul(a, b)));
    }

    ////// 4x1 Vector Functions //////////////////////////////////////////////

    template<bool CMP_W = true>
    inline bool isZero(const block_t& x, const block_t& epsilon0)
    {
      return compareLEQ<CMP_W>(SIMD128::abs(x), epsilon0);
    }

    template<bool CMP_W = true>
    inline bool isZero(const block_t& x, const real_t epsilon0)
    {
      return isZero<CMP_W>(x, SIMD128::set(epsilon0));
    }

    ////// 4x4 Matrix Functions //////////////////////////////////////////////

    namespace impl {

      /*
       * Compute A * B
       */
      inline block_t mul2x2(const block_t& a, const block_t& b)
      {
        using S = SIMD128;
        return S::add(S::mul(                    a,  S::swizzle<0,3,0,3>(b)),
                      S::mul(S::swizzle<1,0,3,2>(a), S::swizzle<2,1,2,1>(b)));
      }

      /*
       * Compute A# * B
       */
      inline block_t adjMul2x2(const block_t& a, const block_t& b)
      {
        using S = SIMD128;
        return S::sub(S::mul(S::swizzle<3,3,0,0>(a),                     b),
                      S::mul(S::swizzle<1,1,2,2>(a), S::swizzle<2,3,0,1>(b)));
      }

      /*
       * Compute A * B#
       */
      inline block_t mulAdj2x2(const block_t& a, const block_t& b)
      {
        using S = SIMD128;
        return S::sub(S::mul(                    a,  S::swizzle<3,0,3,0>(b)),
                      S::mul(S::swizzle<1,0,3,2>(a), S::swizzle<2,1,2,1>(b)));
      }

      /*
       * Compute the trace(A * B)
       */
      inline block_t traceMul2x2(const block_t& a, const block_t& b)
      {
        using S = SIMD128;
        return S::hadd(S::mul(a, S::swizzle<0,2,1,3>(b)));
      }

    } // namespace impl

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
    inline void inverse(real_t *dest, const real_t *src)
    {
      using S = SIMD128;

      const block_t col0 = S::load(src +  0);
      const block_t col1 = S::load(src +  4);
      const block_t col2 = S::load(src +  8);
      const block_t col3 = S::load(src + 12);

      const block_t detACBD = S::sub(
            S::mul(S::shuffle<0,2,0,2>(col0, col2), S::shuffle<1,3,1,3>(col1, col3)),
            S::mul(S::shuffle<1,3,1,3>(col0, col2), S::shuffle<0,2,0,2>(col1, col3))
            );

      const block_t detA = S::swizzle<0,0,0,0>(detACBD);
      const block_t detB = S::swizzle<2,2,2,2>(detACBD);
      const block_t detC = S::swizzle<1,1,1,1>(detACBD);
      const block_t detD = S::swizzle<3,3,3,3>(detACBD);

      const block_t A = S::intrlvlo(col0, col1);
      const block_t B = S::intrlvlo(col2, col3);
      const block_t C = S::intrlvhi(col0, col1);
      const block_t D = S::intrlvhi(col2, col3);

      const block_t AadjB = impl::adjMul2x2(A, B);
      const block_t DadjC = impl::adjMul2x2(D, C);

      // NOTE: Combine adjugate's signs divided by detM.
      const block_t detM = S::div(S::set(1, -1, -1, 1),
                                  S::sub(S::add(S::mul(detA, detD), S::mul(detB, detC)),
                                         impl::traceMul2x2(AadjB, DadjC)));

      const block_t X = S::mul(detM, S::sub(S::mul(detD, A),    impl::mul2x2(B, DadjC)));
      const block_t Y = S::mul(detM, S::sub(S::mul(detB, C), impl::mulAdj2x2(D, AadjB)));
      const block_t Z = S::mul(detM, S::sub(S::mul(detC, B), impl::mulAdj2x2(A, DadjC)));
      const block_t W = S::mul(detM, S::sub(S::mul(detA, D),    impl::mul2x2(C, AadjB)));

      /*
       * NOTE: Account for the adjugate swapping elements along the main diagonal!
       *        [ x3 x1 y3 y1 ]
       * Minv = [ x2 x0 y2 y0 ]
       *        [ z3 z1 w3 w1 ]
       *        [ z2 z0 w2 w0 ]
       */
      S::store(dest +  0, S::shuffle<3,2,3,2>(X, Z));
      S::store(dest +  4, S::shuffle<1,0,1,0>(X, Z));
      S::store(dest +  8, S::shuffle<3,2,3,2>(Y, W));
      S::store(dest + 12, S::shuffle<1,0,1,0>(Y, W));
    }

    /*
     * Linear Transformation of 4x1 Column Vector by 4x4 Column-Major Matrix.
     */
    inline block_t transform(const block_t& col0, const block_t& col1,
                             const block_t& col2, const block_t& col3,
                             const block_t& x)
    {
      using S = SIMD128;
      // NOTE: y = M*x
      block_t   y = S::mul(S::swizzle<0,0,0,0>(x), col0);
      y = S::add(y, S::mul(S::swizzle<1,1,1,1>(x), col1));
      y = S::add(y, S::mul(S::swizzle<2,2,2,2>(x), col2));
      y = S::add(y, S::mul(S::swizzle<3,3,3,3>(x), col3));
      return y;
    }

    /*
     * Transpose of Column-Major 4x4 Matrix.
     */
    inline void transpose(real_t *dest, const real_t *src)
    {
      block_t col0 = SIMD128::load(src +  0);
      block_t col1 = SIMD128::load(src +  4);
      block_t col2 = SIMD128::load(src +  8);
      block_t col3 = SIMD128::load(src + 12);

      const block_t tmp0 = SIMD128::intrlvlo(col0, col2);
      const block_t tmp1 = SIMD128::intrlvhi(col0, col2);
      const block_t tmp2 = SIMD128::intrlvlo(col1, col3);
      const block_t tmp3 = SIMD128::intrlvhi(col1, col3);
      col0 = SIMD128::intrlvlo(tmp0, tmp2);
      col1 = SIMD128::intrlvhi(tmp0, tmp2);
      col2 = SIMD128::intrlvlo(tmp1, tmp3);
      col3 = SIMD128::intrlvhi(tmp1, tmp3);

      SIMD128::store(dest +  0, col0);
      SIMD128::store(dest +  4, col1);
      SIMD128::store(dest +  8, col2);
      SIMD128::store(dest + 12, col3);
    }

    ////// Ray Axis Aligned Bounding Box Intersection ////////////////////////

    inline bool intersectRayAABBox(const block_t& bbMin, const block_t& bbMax,
                                   const block_t& rayOrg, const block_t& rayDir,
                                   const real_t rayMax)
    {
      using S = SIMD128;

      constexpr int XYZ_MASK = impl::CMP_MASK<false>();

      const int result_mask = S::cmp_mask(S::cmp_neq(rayDir, S::zero())) & XYZ_MASK;
      if( result_mask == 0 ) {
        return false;
      }

      const block_t tMin = S::div(S::sub(bbMin, rayOrg), rayDir);
      const block_t tMax = S::div(S::sub(bbMax, rayOrg), rayDir);

      const block_t tRayMax = S::set(rayMax);

      const block_t condMin = S::bit_and(S::cmp_leq(S::zero(), tMin),
                                         S::cmp_lt(tMin, tRayMax));
      const block_t condMax = S::bit_and(S::cmp_leq(S::zero(), tMax),
                                         S::cmp_lt(tMax, tRayMax));

      const int result = S::cmp_mask(S::bit_or(condMin, condMax));

      return (result & result_mask) == result_mask;
    }

    inline bool intersectRayAABBox(const block_t& bbMin, const block_t& bbMax,
                                   const block_t& rayOrg, const block_t& rayDir)
    {
      using S = SIMD128;

      constexpr int XYZ_MASK = impl::CMP_MASK<false>();

      const int result_mask = S::cmp_mask(S::cmp_neq(rayDir, S::zero())) & XYZ_MASK;
      if( result_mask == 0 ) {
        return false;
      }

      const block_t tMin = S::div(S::sub(bbMin, rayOrg), rayDir);
      const block_t tMax = S::div(S::sub(bbMax, rayOrg), rayDir);

      const block_t condMin = S::cmp_leq(S::zero(), tMin);
      const block_t condMax = S::cmp_leq(S::zero(), tMax);

      const int result = S::cmp_mask(S::bit_or(condMin, condMax));

      return (result & result_mask) == result_mask;
    }

  } // namespace simd

} // namespace n4
