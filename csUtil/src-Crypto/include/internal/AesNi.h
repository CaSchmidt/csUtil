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

#include <wmmintrin.h>

#include <cs/SIMD/SIMD128.h>

#include "internal/Aes.h"

namespace cs {

  namespace impl_aes {

    ////// AES-NI SIMD Support ///////////////////////////////////////////////

    struct AESNI : public SIMD128<int32_t> {
      template<int RCON>
      inline static block_type aeskeygenassist(const block_type& w)
      {
        return _mm_aeskeygenassist_si128(w, RCON);
      }
    };

    ////// AES Key Expansion /////////////////////////////////////////////////

    template<int BITS, std::size_t COUNTER>
    struct KeyExpansion {
      using Traits = AesTraits<BITS>;

      static_assert( 0 < COUNTER  &&  COUNTER <= Traits::NUM_KEYEXP );

      template<std::size_t I>
      inline static void eval(uint32_t *w)
      {
        using S = AESNI;

        constexpr std::size_t ZERO = 0;
        constexpr std::size_t  ONE = 1;
        constexpr std::size_t FOUR = 4;

        uint32_t temp = w[I - ONE];

        if constexpr( I%Traits::Nk == ZERO ) {
          constexpr int RCON = AesRCON<I/Traits::Nk>::value;
          const S::block_type TEMP =
              S::aeskeygenassist<RCON>(S::load<false>(&w[I - FOUR]));
          temp = static_cast<uint32_t>(S::to_value(S::swizzle<3,3,3,3>(TEMP)));
        }

        w[I] = w[I - Traits::Nk] ^ temp;
      }

      inline static void loop(uint32_t *w)
      {
        constexpr std::size_t I = Traits::NUM_KEYEXP - COUNTER + Traits::Nk;

        eval<I>(w);

        KeyExpansion<BITS,COUNTER-1>::loop(w);
      }
    };

    template<int BITS>
    struct KeyExpansion<BITS,0> {
      using Traits = AesTraits<BITS>;

      inline static void loop(uint32_t *)
      {
      }

      inline static void start(uint32_t *w, const byte_t *key)
      {
        constexpr std::size_t FOUR = 4;

        for(std::size_t i = 0; i < Traits::Nk; i++) {
          w[i] = *reinterpret_cast<const uint32_t*>(&key[i*FOUR]);
        }

        KeyExpansion<BITS,Traits::NUM_KEYEXP>::loop(w);
      }
    };

  } // namespace impl_aes

} // namespace cs
