/****************************************************************************
** Copyright (c) 2022, Carsten Schmidt. All rights reserved.
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

#ifndef CSSIMD128_H
#define CSSIMD128_H

#include <csUtil/csConcepts.h>
#include <csUtil/csSIMD128Util.h>

namespace cs {

  template<int SIZE>
  struct simd128_impl {
    // SFINAE
  };

  template<>
  struct simd128_impl<4> {
    using block_type = __m128;
    using  size_type = std::size_t;
    using value_type = float;

    static constexpr size_type NUM_ELEMS = sizeof(block_type)/sizeof(value_type);

    static_assert( NUM_ELEMS == 4 );

    ////// Functions ///////////////////////////////////////////////////////

    inline static block_type add(const block_type& a, const block_type& b)
    {
      return _mm_add_ps(a, b);
    }

    inline static block_type hadd(const block_type& x)
    {
      const block_type y = _mm_add_ps(x, simd128_swizzle_ps<1,0,3,2>(x));
      return               _mm_add_ps(y, simd128_swizzle_ps<3,2,1,0>(y));
    }

    template<bool ALIGNED = true>
    inline static block_type load(const value_type *ptr)
    {
      if constexpr( ALIGNED ) {
        return _mm_load_ps(ptr);
      }
      return _mm_loadu_ps(ptr);
    }

    inline static block_type mul(const block_type& a, const block_type& b)
    {
      return _mm_mul_ps(a, b);
    }

    inline static void prefetch(const value_type *ptr)
    {
      _mm_prefetch(reinterpret_cast<const char*>(ptr), _MM_HINT_NTA);
    }

    inline static value_type to_value(const block_type& x)
    {
      return _mm_cvtss_f32(x);
    }

    inline static block_type zero()
    {
      return _mm_setzero_ps();
    }
  };

  template<>
  struct simd128_impl<8> {
    using block_type = __m128d;
    using  size_type = std::size_t;
    using value_type = double;

    static constexpr size_type NUM_ELEMS = sizeof(block_type)/sizeof(value_type);

    static_assert( NUM_ELEMS == 2 );

    ////// Functions ///////////////////////////////////////////////////////

    inline static block_type add(const block_type& a, const block_type& b)
    {
      return _mm_add_pd(a, b);
    }

    inline static block_type hadd(const block_type& x)
    {
      return _mm_add_pd(x, simd128_swizzle_pd<1,0>(x));
    }

    template<bool ALIGNED = true>
    inline static block_type load(const value_type *ptr)
    {
      if constexpr( ALIGNED ) {
        return _mm_load_pd(ptr);
      }
      return _mm_loadu_pd(ptr);
    }

    inline static block_type mul(const block_type& a, const block_type& b)
    {
      return _mm_mul_pd(a, b);
    }

    inline static void prefetch(const value_type *ptr)
    {
      _mm_prefetch(reinterpret_cast<const char*>(ptr), _MM_HINT_NTA);
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

  template<typename T> requires IsReal<T>
  using simd128_type = simd128_impl<sizeof(T)>;

} // namespace cs

#endif // CSSIMD128_H
