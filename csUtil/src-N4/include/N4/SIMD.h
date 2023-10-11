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

#ifndef N4_SIMD_H
#define N4_SIMD_H

#include <emmintrin.h> // SSE2

namespace simd {

  ////// Types ///////////////////////////////////////////////////////////////

  using real_t = float;

  using simd_t = __m128;

  ////// Macros //////////////////////////////////////////////////////////////

#define SIMD_SHIFTR(vec,bits) \
  _mm_castsi128_ps(_mm_srli_epi32(_mm_castps_si128(vec), bits))

#define SIMD_SHIFTLx8(vec,bytes) \
  _mm_castsi128_ps(_mm_slli_si128(_mm_castps_si128(vec), bytes))

#define SIMD_SHIFTRx8(vec,bytes) \
  _mm_castsi128_ps(_mm_srli_si128(_mm_castps_si128(vec), bytes))

#define SIMD_SHUFFLE(a,b,a1,a2,b1,b2) \
  _mm_shuffle_ps(a, b, _MM_SHUFFLE((b2), (b1), (a2), (a1)))

#define SIMD_SWIZZLE(vec,x,y,z,w) \
  _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vec), _MM_SHUFFLE((w), (z), (y), (x))))

  ////// Elementary Functions ////////////////////////////////////////////////

  inline simd_t abs_mask()
  {
    const simd_t zero = _mm_setzero_ps();
    return SIMD_SHIFTR(_mm_cmpeq_ps(zero, zero), 1);
  }

  inline simd_t abs(const simd_t& x)
  {
    return _mm_and_ps(x, abs_mask());
  }

  inline simd_t add(const simd_t& a, const simd_t& b)
  {
    return _mm_add_ps(a, b);
  }

  inline simd_t bit_and(const simd_t& a, const simd_t& b)
  {
    return _mm_and_ps(a, b);
  }

  inline simd_t bit_andnot(const simd_t& a, const simd_t& b)
  {
    return _mm_andnot_ps(a, b); // NOTE: ~a & b
  }

  inline simd_t bit_or(const simd_t& a, const simd_t& b)
  {
    return _mm_or_ps(a, b);
  }

  inline simd_t bit_xor(const simd_t& a, const simd_t& b)
  {
    return _mm_xor_ps(a, b);
  }

  inline simd_t clamp(const simd_t& x, const simd_t& lo, const simd_t& hi)
  {
    return _mm_max_ps(lo, _mm_min_ps(x, hi));
  }

  inline simd_t cmpEQ(const simd_t& a, const simd_t& b)
  {
    return _mm_cmpeq_ps(a, b);
  }

  inline simd_t cmpGEQ(const simd_t& a, const simd_t& b)
  {
    return _mm_cmpge_ps(a, b);
  }

  inline simd_t cmpGT(const simd_t& a, const simd_t& b)
  {
    return _mm_cmpgt_ps(a, b);
  }

  inline simd_t cmpLEQ(const simd_t& a, const simd_t& b)
  {
    return _mm_cmple_ps(a, b);
  }

  inline simd_t cmpLT(const simd_t& a, const simd_t& b)
  {
    return _mm_cmplt_ps(a, b);
  }

  inline int cmpMask(const simd_t& x)
  {
    return _mm_movemask_ps(x);
  }

  inline simd_t cmpNEQ(const simd_t& a, const simd_t& b)
  {
    return _mm_cmpneq_ps(a, b);
  }

  inline simd_t div(const simd_t& a, const simd_t& b)
  {
    return _mm_div_ps(a, b);
  }

  inline simd_t hadd(const simd_t& x)
  {
    const simd_t y = _mm_add_ps(x, SIMD_SWIZZLE(x, 1, 0, 3, 2));
    return           _mm_add_ps(y, SIMD_SWIZZLE(y, 3, 2, 1, 0));
  }

  inline simd_t load(const real_t *ptr)
  {
    return _mm_load_ps(ptr);
  }

  inline simd_t max(const simd_t& a, const simd_t& b)
  {
    return _mm_max_ps(a, b);
  }

  inline simd_t min(const simd_t& a, const simd_t& b)
  {
    return _mm_min_ps(a, b);
  }

  inline simd_t mul(const simd_t& a, const simd_t& b)
  {
    return _mm_mul_ps(a, b);
  }

  inline simd_t set(const real_t val)
  {
    return _mm_set1_ps(val);
  }

  inline simd_t set(const real_t x, const real_t y, const real_t z, const real_t w)
  {
    return _mm_set_ps(w, z, y, x);
  }

  inline simd_t sqrt(const simd_t& x)
  {
    return _mm_sqrt_ps(x);
  }

  inline void store(real_t *ptr, const simd_t& x)
  {
    _mm_store_ps(ptr, x);
  }

  inline simd_t sub(const simd_t& a, const simd_t& b)
  {
    return _mm_sub_ps(a, b);
  }

  inline real_t to_real(const simd_t& x)
  {
    return _mm_cvtss_f32(x);
  }

  inline simd_t intrlvhi(const simd_t& a, const simd_t& b)
  {
    return _mm_unpackhi_ps(a, b);
  }

  inline simd_t intrlvlo(const simd_t& a, const simd_t& b)
  {
    return _mm_unpacklo_ps(a, b);
  }

  inline simd_t zero()
  {
    return _mm_setzero_ps();
  }

  inline simd_t zero_w(const simd_t& x)
  {
    return SIMD_SHIFTRx8(SIMD_SHIFTLx8(x, 4), 4);
  }

  ////// Relations ///////////////////////////////////////////////////////////

  inline simd_t cmov(const simd_t& condA, const simd_t& a, const simd_t& b)
  {
    return bit_or(bit_and(condA, a), bit_andnot(condA, b));
  }

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
  inline bool compareLEQ(const simd_t& a, const simd_t& b)
  {
    constexpr int MASK = impl::CMP_MASK<CMP_W>();
    return (cmpMask(cmpLEQ(a, b)) & MASK) == MASK;
  }

  ////// 3x1 Vector Functions ////////////////////////////////////////////////

  /*
   *         a0   b0   a1*b2 - a2*b1
   * a X b = a1 X b1 = a2*b0 - a0*b2
   *         a2   b2   a0*b1 - a1*b0
   */
  inline simd_t cross(const simd_t& a, const simd_t& b)
  {
    const simd_t prod1 = mul(SIMD_SWIZZLE(a, 1, 2, 0, 3),
                             SIMD_SWIZZLE(b, 2, 0, 1, 3));
    const simd_t prod2 = mul(SIMD_SWIZZLE(a, 2, 0, 1, 3),
                             SIMD_SWIZZLE(b, 1, 2, 0, 3));
    return sub(prod1, prod2);
  }

  inline simd_t dot3(const simd_t& a, const simd_t& b)
  {
    return hadd(zero_w(mul(a, b)));
  }

  ////// 4x1 Vector Functions ////////////////////////////////////////////////

  template<bool CMP_W = true>
  inline bool isZero(const simd_t& x, const simd_t& epsilon0)
  {
    return compareLEQ<CMP_W>(abs(x), epsilon0);
  }

  template<bool CMP_W = true>
  inline bool isZero(const simd_t& x, const real_t epsilon0)
  {
    return isZero<CMP_W>(x, set(epsilon0));
  }

  ////// 4x4 Matrix Functions ////////////////////////////////////////////////

  namespace impl {

    /*
     * Compute A * B
     */
    inline simd_t mul2x2(const simd_t& a, const simd_t& b)
    {
      return add(mul(             a,              SIMD_SWIZZLE(b, 0, 3, 0, 3)),
                 mul(SIMD_SWIZZLE(a, 1, 0, 3, 2), SIMD_SWIZZLE(b, 2, 1, 2, 1)));
    }

    /*
     * Compute A# * B
     */
    inline simd_t adjMul2x2(const simd_t& a, const simd_t& b)
    {
      return sub(mul(SIMD_SWIZZLE(a, 3, 3, 0, 0),              b),
                 mul(SIMD_SWIZZLE(a, 1, 1, 2, 2), SIMD_SWIZZLE(b, 2, 3, 0, 1)));
    }

    /*
     * Compute A * B#
     */
    inline simd_t mulAdj2x2(const simd_t& a, const simd_t& b)
    {
      return sub(mul(             a,              SIMD_SWIZZLE(b, 3, 0, 3, 0)),
                 mul(SIMD_SWIZZLE(a, 1, 0, 3, 2), SIMD_SWIZZLE(b, 2, 1, 2, 1)));
    }

    /*
     * Compute the trace(A * B)
     */
    inline simd_t traceMul2x2(const simd_t& a, const simd_t& b)
    {
      return hadd(mul(a, SIMD_SWIZZLE(b, 0, 2, 1, 3)));
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
    const simd_t col0 = load(src +  0);
    const simd_t col1 = load(src +  4);
    const simd_t col2 = load(src +  8);
    const simd_t col3 = load(src + 12);

    const simd_t detACBD = sub(
          mul(SIMD_SHUFFLE(col0, col2, 0, 2, 0, 2), SIMD_SHUFFLE(col1, col3, 1, 3, 1, 3)),
          mul(SIMD_SHUFFLE(col0, col2, 1, 3, 1, 3), SIMD_SHUFFLE(col1, col3, 0, 2, 0, 2))
          );

    const simd_t detA = SIMD_SWIZZLE(detACBD, 0, 0, 0, 0);
    const simd_t detB = SIMD_SWIZZLE(detACBD, 2, 2, 2, 2);
    const simd_t detC = SIMD_SWIZZLE(detACBD, 1, 1, 1, 1);
    const simd_t detD = SIMD_SWIZZLE(detACBD, 3, 3, 3, 3);

    const simd_t A = intrlvlo(col0, col1);
    const simd_t B = intrlvlo(col2, col3);
    const simd_t C = intrlvhi(col0, col1);
    const simd_t D = intrlvhi(col2, col3);

    const simd_t AadjB = impl::adjMul2x2(A, B);
    const simd_t DadjC = impl::adjMul2x2(D, C);

    // NOTE: Combine adjugate's signs divided by detM.
    const simd_t detM = div(set(1, -1, -1, 1),
                            sub(add(mul(detA, detD), mul(detB, detC)),
                                impl::traceMul2x2(AadjB, DadjC)));

    const simd_t X = mul(detM, sub(mul(detD, A),    impl::mul2x2(B, DadjC)));
    const simd_t Y = mul(detM, sub(mul(detB, C), impl::mulAdj2x2(D, AadjB)));
    const simd_t Z = mul(detM, sub(mul(detC, B), impl::mulAdj2x2(A, DadjC)));
    const simd_t W = mul(detM, sub(mul(detA, D),    impl::mul2x2(C, AadjB)));

    /*
     * NOTE: Account for the adjugate swapping elements along the main diagonal!
     *        [ x3 x1 y3 y1 ]
     * Minv = [ x2 x0 y2 y0 ]
     *        [ z3 z1 w3 w1 ]
     *        [ z2 z0 w2 w0 ]
     */
    store(dest +  0, SIMD_SHUFFLE(X, Z, 3, 2, 3, 2));
    store(dest +  4, SIMD_SHUFFLE(X, Z, 1, 0, 1, 0));
    store(dest +  8, SIMD_SHUFFLE(Y, W, 3, 2, 3, 2));
    store(dest + 12, SIMD_SHUFFLE(Y, W, 1, 0, 1, 0));
  }

  /*
   * Linear Transformation of 4x1 Column Vector by 4x4 Column-Major Matrix.
   */
  inline simd_t transform(const simd_t& col0, const simd_t& col1,
                          const simd_t& col2, const simd_t& col3,
                          const simd_t& x)
  {
    // NOTE: y = M*x
    simd_t y = mul(SIMD_SWIZZLE(x, 0, 0, 0, 0), col0);
    y = add(y, mul(SIMD_SWIZZLE(x, 1, 1, 1, 1), col1));
    y = add(y, mul(SIMD_SWIZZLE(x, 2, 2, 2, 2), col2));
    y = add(y, mul(SIMD_SWIZZLE(x, 3, 3, 3, 3), col3));
    return y;
  }

  /*
   * Transpose of Column-Major 4x4 Matrix.
   */
  inline void transpose(real_t *dest, const real_t *src)
  {
    simd::simd_t col0 = load(src +  0);
    simd::simd_t col1 = load(src +  4);
    simd::simd_t col2 = load(src +  8);
    simd::simd_t col3 = load(src + 12);

    const simd_t tmp0 = intrlvlo(col0, col2);
    const simd_t tmp1 = intrlvhi(col0, col2);
    const simd_t tmp2 = intrlvlo(col1, col3);
    const simd_t tmp3 = intrlvhi(col1, col3);
    col0 = intrlvlo(tmp0, tmp2);
    col1 = intrlvhi(tmp0, tmp2);
    col2 = intrlvlo(tmp1, tmp3);
    col3 = intrlvhi(tmp1, tmp3);

    simd::store(dest +  0, col0);
    simd::store(dest +  4, col1);
    simd::store(dest +  8, col2);
    simd::store(dest + 12, col3);
  }

  ////// Ray Axis Aligned Bounding Box Intersection //////////////////////////

  inline bool intersectRayAABBox(const simd_t& bbMin, const simd_t& bbMax,
                                 const simd_t& rayOrg, const simd_t& rayDir,
                                 const real_t rayMax)
  {
    constexpr int XYZ_MASK = impl::CMP_MASK<false>();

    const int result_mask = cmpMask(cmpNEQ(rayDir, zero())) & XYZ_MASK;
    if( result_mask == 0 ) {
      return false;
    }

    const simd_t tMin = div(sub(bbMin, rayOrg), rayDir);
    const simd_t tMax = div(sub(bbMax, rayOrg), rayDir);

    const simd_t tRayMax = set(rayMax);

    const simd_t condMin = bit_and(cmpLEQ(zero(), tMin), cmpLT(tMin, tRayMax));
    const simd_t condMax = bit_and(cmpLEQ(zero(), tMax), cmpLT(tMax, tRayMax));

    const int result = cmpMask(bit_or(condMin, condMax));

    return (result & result_mask) == result_mask;
  }

  inline bool intersectRayAABBox(const simd_t& bbMin, const simd_t& bbMax,
                                 const simd_t& rayOrg, const simd_t& rayDir)
  {
    constexpr int XYZ_MASK = impl::CMP_MASK<false>();

    const int result_mask = cmpMask(cmpNEQ(rayDir, zero())) & XYZ_MASK;
    if( result_mask == 0 ) {
      return false;
    }

    const simd_t tMin = div(sub(bbMin, rayOrg), rayDir);
    const simd_t tMax = div(sub(bbMax, rayOrg), rayDir);

    const simd_t condMin = cmpLEQ(zero(), tMin);
    const simd_t condMax = cmpLEQ(zero(), tMax);

    const int result = cmpMask(bit_or(condMin, condMax));

    return (result & result_mask) == result_mask;
  }

} // namespace simd

#endif // N4_SIMD_H
