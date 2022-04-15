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

#ifndef CSSIMD128UTIL_H
#define CSSIMD128UTIL_H

#include <emmintrin.h>

template<int E0, int E1, int E2, int E3>
constexpr int simd128_shuffle_mask()
{
  static_assert( 0 <= E0  &&  E0 <= 3 );
  static_assert( 0 <= E1  &&  E1 <= 3 );
  static_assert( 0 <= E2  &&  E2 <= 3 );
  static_assert( 0 <= E3  &&  E3 <= 3 );

  return (E3 << 6) | (E2 << 4) | (E1 << 2) | E0;
}

template<int E0, int E1>
inline __m128d simd128_swizzle_pd(const __m128d& x)
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

template<int E0, int E1, int E2, int E3>
inline __m128 simd128_swizzle_ps(const __m128& x)
{
  constexpr int MASK = simd128_shuffle_mask<E0,E1,E2,E3>();

  return _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(x), MASK));
}

#endif // CSSIMD128UTIL_H
