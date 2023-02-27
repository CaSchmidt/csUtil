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
  struct simd128_impl<4,true> {
    using block_type = __m128;
    using  size_type = std::size_t;
    using value_type = RealOfSize<4>::real_type;

    static constexpr size_type NUM_ELEMS = sizeof(block_type)/sizeof(value_type);

    static_assert( NUM_ELEMS == 4 );

    ////// Functions /////////////////////////////////////////////////////////

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
      return ALIGNED
          ? _mm_load_ps(ptr)
          : _mm_loadu_ps(ptr);
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

} // namespace cs