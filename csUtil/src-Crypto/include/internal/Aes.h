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

#include "cs/Core/TypeTraits.h"

namespace cs {

  namespace impl_aes {

    ////// AES Constants /////////////////////////////////////////////////////

    template<std::size_t VALUE>
    using aes_constant = std::integral_constant<std::size_t,VALUE>;

    template<int BITS>
    struct AesNb { /* SFINAE */ };
    template<>
    struct AesNb<128> : public aes_constant<4> {};
    template<>
    struct AesNb<192> : public aes_constant<4> {};
    template<>
    struct AesNb<256> : public aes_constant<4> {};

    template<int BITS>
    struct AesNk { /* SFINAE */ };
    template<>
    struct AesNk<128> : public aes_constant<4> {};
    template<>
    struct AesNk<192> : public aes_constant<6> {};
    template<>
    struct AesNk<256> : public aes_constant<8> {};

    template<int BITS>
    struct AesNr { /* SFINAE */ };
    template<>
    struct AesNr<128> : public aes_constant<10> {};
    template<>
    struct AesNr<192> : public aes_constant<12> {};
    template<>
    struct AesNr<256> : public aes_constant<14> {};

    template<std::size_t I>
    struct AesRCON { /* SFINAE */ };
    template<>
    struct AesRCON< 1> : public std::integral_constant<int,0x01> {};
    template<>
    struct AesRCON< 2> : public std::integral_constant<int,0x02> {};
    template<>
    struct AesRCON< 3> : public std::integral_constant<int,0x04> {};
    template<>
    struct AesRCON< 4> : public std::integral_constant<int,0x08> {};
    template<>
    struct AesRCON< 5> : public std::integral_constant<int,0x10> {};
    template<>
    struct AesRCON< 6> : public std::integral_constant<int,0x20> {};
    template<>
    struct AesRCON< 7> : public std::integral_constant<int,0x40> {};
    template<>
    struct AesRCON< 8> : public std::integral_constant<int,0x80> {};
    template<>
    struct AesRCON< 9> : public std::integral_constant<int,0x1B> {};
    template<>
    struct AesRCON<10> : public std::integral_constant<int,0x36> {};

    ////// AES Implementation Traits /////////////////////////////////////////

    template<int BITS>
    struct AesTraits {
      static constexpr std::size_t ONE = 1;

      static constexpr std::size_t Nb = AesNb<BITS>::value;
      static constexpr std::size_t Nk = AesNk<BITS>::value;
      static constexpr std::size_t Nr = AesNr<BITS>::value;

      static constexpr std::size_t NUM_KEYEXP = Nb*(Nr + ONE) - Nk;
    };

  } // namespace impl_aes

} // namespace cs
