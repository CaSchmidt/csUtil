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

#include <cs/SIMD/SIMD128Impl.h>
#include <cs/SIMD/x86/SIMD128Util.h>

namespace cs {

  template<>
  struct simd128_impl<8,true> {
    using block_type = __m128d;
    using  size_type = std::size_t;
    using value_type = RealOfSize<8>::real_type;

    static constexpr size_type NUM_ELEM = sizeof(block_type)/sizeof(value_type);

    static_assert( NUM_ELEM == 2 );

    ////// Functions /////////////////////////////////////////////////////////

    inline static block_type add(const block_type& a, const block_type& b)
    {
      return _mm_add_pd(a, b);
    }

    inline static block_type hadd(const block_type& x)
    {
      return _mm_add_pd(x, swizzle<1,0>(x));
    }

    template<bool ALIGNED = true>
    inline static block_type load(const value_type *ptr)
    {
      return ALIGNED
          ? _mm_load_pd(ptr)
          : _mm_loadu_pd(ptr);
    }

    inline static block_type mul(const block_type& a, const block_type& b)
    {
      return _mm_mul_pd(a, b);
    }

    inline static void prefetch(const value_type *ptr)
    {
      _mm_prefetch(reinterpret_cast<const char*>(ptr), _MM_HINT_NTA);
    }

    template<int BITS>
    inline static block_type shiftl(const block_type& x)
    {
      static_assert( 0 <= BITS  &&  BITS <= 63 );

      return _mm_castsi128_pd(_mm_slli_epi64(_mm_castpd_si128(x), BITS));
    }

    template<int BYTES>
    inline static block_type shiftl8(const block_type& x)
    {
      static_assert( 0 <= BYTES  &&  BYTES <= 15 );

      return _mm_castsi128_pd(_mm_slli_si128(_mm_castpd_si128(x), BYTES));
    }

    template<int BITS>
    inline static block_type shiftr(const block_type& x)
    {
      static_assert( 0 <= BITS  &&  BITS <= 63 );

      return _mm_castsi128_pd(_mm_srli_epi64(_mm_castpd_si128(x), BITS));
    }

    template<int BYTES>
    inline static block_type shiftr8(const block_type& x)
    {
      static_assert( 0 <= BYTES  &&  BYTES <= 15 );

      return _mm_castsi128_pd(_mm_srli_si128(_mm_castpd_si128(x), BYTES));
    }

    template<int A0, int B1>
    inline static block_type shuffle(const block_type& a, const block_type& b)
    {
      static_assert( 0 <= A0  &&  A0 <= 1 );
      static_assert( 0 <= B1  &&  B1 <= 1 );

      constexpr int MASK = (B1 << 1) | A0;

      return _mm_shuffle_pd(a, b, MASK);
    }

    template<int E0, int E1>
    inline static block_type swizzle(const block_type& x)
    {
      static_assert( 0 <= E0  &&  E0 <= 1 );
      static_assert( 0 <= E1  &&  E1 <= 1 );

      constexpr int M0 = E0*2;
      constexpr int M1 = E0*2 + 1;
      constexpr int M2 = E1*2;
      constexpr int M3 = E1*2 + 1;

      constexpr int MASK = simd128_shuffle_mask<M0,M1,M2,M3>();

      return _mm_castsi128_pd(_mm_shuffle_epi32(_mm_castpd_si128(x), MASK));
    }

    inline static value_type to_value(const block_type& x)
    {
      return _mm_cvtsd_f64(x);
    }

    inline static block_type zero()
    {
      return _mm_setzero_pd();
    }
  };

} // namespace cs
