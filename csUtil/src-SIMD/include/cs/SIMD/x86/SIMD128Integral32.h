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

#include <emmintrin.h>

#include <cs/Core/Bit.h>
#include <cs/SIMD/SIMD128Impl.h>
#include <cs/SIMD/x86/SIMD128Util.h>

namespace cs {

  template<>
  struct SIMD128Impl<4,SIMD128_IS_INTEGRAL> {
    using block_type = __m128i;
    using  size_type = std::size_t;
    using value_type = IntegralOfSize<4>::signed_type;

    static constexpr size_type NUM_ELEM = sizeof(block_type)/sizeof(value_type);

    static_assert( NUM_ELEM == 4 );

    ////// Arithmetic ////////////////////////////////////////////////////////

    inline static block_type add(const block_type& a, const block_type& b)
    {
      return _mm_add_epi32(a, b);
    }

    inline static block_type hadd(const block_type& x)
    {
      return simd::hadd<SIMD128Impl>(x);
    }

    inline static block_type sub(const block_type& a, const block_type& b)
    {
      return _mm_sub_epi32(a, b);
    }

    ////// Assignment, Load & Store //////////////////////////////////////////

    template<bool ALIGNED = true>
    inline static block_type load(const value_type *src)
    {
      return ALIGNED
          ? _mm_load_si128(reinterpret_cast<const __m128i*>(src))
          : _mm_loadu_si128(reinterpret_cast<const __m128i*>(src));
    }

    template<bool ALIGNED = true>
    inline static block_type load(const void *src)
    {
      return load<ALIGNED>(reinterpret_cast<const value_type*>(src));
    }

    inline static block_type set(const value_type val)
    {
      return _mm_set1_epi32(val);
    }

    inline static block_type set(const value_type e0, const value_type e1,
                                 const value_type e2, const value_type e3)
    {
      return _mm_set_epi32(e3, e2, e1, e0);
    }

    template<bool ALIGNED = true>
    inline static void store(value_type *dest, const block_type& x)
    {
      return ALIGNED
          ? _mm_store_si128(reinterpret_cast<__m128i*>(dest), x)
          : _mm_storeu_si128(reinterpret_cast<__m128i*>(dest), x);
    }

    template<bool ALIGNED = true>
    inline static void store(void *dest, const block_type& x)
    {
      store<ALIGNED>(reinterpret_cast<value_type*>(dest), x);
    }

    inline static value_type to_value(const block_type& x)
    {
      return _mm_cvtsi128_si32(x);
    }

    ////// Bit Operations ////////////////////////////////////////////////////

    inline static block_type bit_and(const block_type& a, const block_type& b)
    {
      return _mm_and_si128(a, b);
    }

    inline static block_type bit_andnot(const block_type& a, const block_type& b)
    {
      return _mm_andnot_si128(a, b); // NOTE: ~a & b
    }

    inline static block_type bit_or(const block_type& a, const block_type& b)
    {
      return _mm_or_si128(a, b);
    }

    inline static block_type bit_xor(const block_type& a, const block_type& b)
    {
      return _mm_xor_si128(a, b);
    }

    inline static int cmp_mask(const block_type& x)
    {
      return sign_mask(x);
    }

    inline static block_type one()
    {
      return simd::one<SIMD128Impl>();
    }

    inline static int sign_mask(const block_type& x)
    {
      const int mask = _mm_movemask_epi8(x);
      return
          moveBitR<15,3>(mask) | moveBitR<11,2>(mask) |
          moveBitR<7,1>(mask)  | moveBitR<3,0>(mask);
    }

    inline static block_type zero()
    {
      return _mm_setzero_si128();
    }

    ////// Data Flow /////////////////////////////////////////////////////////

    inline static void prefetch(const value_type *ptr)
    {
      _mm_prefetch(reinterpret_cast<const char*>(ptr), _MM_HINT_NTA);
    }

    ////// Math //////////////////////////////////////////////////////////////

    inline static block_type abs(const block_type& x)
    {
      return simd::abs<SIMD128Impl>(x);
    }

    ////// Relations /////////////////////////////////////////////////////////

    inline static block_type cmov(const block_type& cond_a,
                                  const block_type& a, const block_type& b)
    {
      return simd::cmov<SIMD128Impl>(cond_a, a, b);
    }

    inline static block_type cmp_eq(const block_type& a, const block_type& b)
    {
      return _mm_cmpeq_epi32(a, b);
    }

    inline static block_type cmp_gt(const block_type& a, const block_type& b)
    {
      return _mm_cmpgt_epi32(a, b);
    }

    inline static block_type cmp_lt(const block_type& a, const block_type& b)
    {
      return _mm_cmplt_epi32(a, b);
    }

    ////// Shift /////////////////////////////////////////////////////////////

    template<int BITS>
    inline static block_type shiftl(const block_type& x)
    {
      static_assert( 0 <= BITS  &&  BITS <= 31 );

      return _mm_slli_epi32(x, BITS);
    }

    template<int BYTES>
    inline static block_type shiftl8(const block_type& x)
    {
      static_assert( 0 <= BYTES  &&  BYTES <= 15 );

      return _mm_slli_si128(x, BYTES);
    }

    template<int BITS>
    inline static block_type shiftr(const block_type& x)
    {
      static_assert( 0 <= BITS  &&  BITS <= 31 );

      return _mm_srli_epi32(x, BITS);
    }

    template<int BYTES>
    inline static block_type shiftr8(const block_type& x)
    {
      static_assert( 0 <= BYTES  &&  BYTES <= 15 );

      return _mm_srli_si128(x, BYTES);
    }

    ////// Shuffle ///////////////////////////////////////////////////////////

    inline static block_type intrlvhi(const block_type& a, const block_type& b)
    {
      return _mm_unpackhi_epi32(a, b);
    }

    inline static block_type intrlvlo(const block_type& a, const block_type& b)
    {
      return _mm_unpacklo_epi32(a, b);
    }

    template<int E0, int E1, int E2, int E3>
    inline static block_type swizzle(const block_type& x)
    {
      constexpr int MASK = simd128_shuffle_mask<E0,E1,E2,E3>();

      return _mm_shuffle_epi32(x, MASK);
    }
  };

} // namespace cs
