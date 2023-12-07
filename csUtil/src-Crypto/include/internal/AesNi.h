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
      inline static block_type aesdec(const block_type& cipher, const block_type& key)
      {
        return _mm_aesdec_si128(cipher, key);
      }

      inline static block_type aesdeclast(const block_type& cipher, const block_type& key)
      {
        return _mm_aesdeclast_si128(cipher, key);
      }

      inline static block_type aesenc(const block_type& plain, const block_type& key)
      {
        return _mm_aesenc_si128(plain, key);
      }

      inline static block_type aesenclast(const block_type& plain, const block_type& key)
      {
        return _mm_aesenclast_si128(plain, key);
      }

      inline static block_type aesimc(const block_type& w)
      {
        return _mm_aesimc_si128(w);
      }

      template<int RCON>
      inline static block_type aeskeygenassist(const block_type& w)
      {
        return _mm_aeskeygenassist_si128(w, RCON);
      }
    };

    ////// AES Key Expansion /////////////////////////////////////////////////

    template<typename Traits, size_t COUNTER>
    struct KeyExpansion {
      static_assert( 0 < COUNTER  &&  COUNTER <= Traits::NUM_KEYEXPITER );

      using word_t = typename Traits::word_t;

      template<size_t I>
      inline static void eval(word_t *w)
      {
        word_t temp = w[I - ONE];

        if constexpr( I%Nk == ZERO ) {
          temp = keygenassist<I,3>(w);
        } else if constexpr( Nk > SIX  &&  I%Nk == FOUR ) {
          temp = keygenassist<I,2>(w);
        }

        w[I] = w[I - Nk] ^ temp;
      }

      template<size_t I, int SEL>
      inline static word_t keygenassist(const word_t *w)
      {
        constexpr int RCON = AesRCON<I/Nk>::value;

        const S::block_type  in = S::load<false>(&w[I - FOUR]);
        const S::block_type out = S::aeskeygenassist<RCON>(in);

        return static_cast<word_t>(S::to_value(S::swizzle<SEL,SEL,SEL,SEL>(out)));
      }

      inline static void loop(word_t *w)
      {
        constexpr size_t I = Traits::NUM_KEYEXPITER - COUNTER + Nk;

        eval<I>(w);

        KeyExpansion<Traits,COUNTER-1>::loop(w);
      }

    private:
      using S = AESNI;

      static constexpr size_t Nk = Traits::Nk;

      static constexpr size_t ZERO = 0;
      static constexpr size_t  ONE = 1;
      static constexpr size_t FOUR = 4;
      static constexpr size_t  SIX = 6;
    };

    template<typename Traits>
    struct KeyExpansion<Traits,0> {
      using word_t = typename Traits::word_t;

      inline static void loop(word_t *)
      {
      }

      inline static void run(word_t *w, const byte_t *key)
      {
        constexpr size_t FOUR = 4;

        for(size_t i = 0; i < Nk; i++) {
          w[i] = *reinterpret_cast<const word_t*>(&key[i*FOUR]);
        }

        KeyExpansion<Traits,Traits::NUM_KEYEXPITER>::loop(w);
      }

    private:
      static constexpr size_t Nk = Traits::Nk;
    };

    ////// AES Decrypt Keys //////////////////////////////////////////////////

    template<typename Traits>
    inline void setDecryptKeys(typename Traits::word_t *dec,
                               const typename Traits::word_t *enc)
    {
      using S = AESNI;

      constexpr size_t Nb = Traits::Nb;
      constexpr size_t Nr = Traits::Nr;

      constexpr size_t ZERO = 0;
      constexpr size_t  ONE = 1;

      S::store(&dec[ZERO*Nb], S::load(&enc[Nr*Nb]));
      for(size_t i = ONE; i < Nr; i++) {
        S::store(&dec[i*Nb], S::aesimc(S::load(&enc[(Nr - i)*Nb])));
      }
      S::store(&dec[Nr*Nb], S::load(&enc[ZERO*Nb]));
    }

  } // namespace impl_aes

} // namespace cs
