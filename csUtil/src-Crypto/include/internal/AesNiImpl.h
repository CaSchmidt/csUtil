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

#include <array>

#include "cs/Crypto/BlockCipher.h"
#include "internal/AesNi.h"

namespace cs {

  namespace impl_aes {

    template<int BITS>
    struct AesAlg { /* SFINAE */ };
    template<>
    struct AesAlg<128> : std::integral_constant<BlockCipher::Algorithm,BlockCipher::AES128> {};
    template<>
    struct AesAlg<192> : std::integral_constant<BlockCipher::Algorithm,BlockCipher::AES192> {};
    template<>
    struct AesAlg<256> : std::integral_constant<BlockCipher::Algorithm,BlockCipher::AES256> {};

  } // namespace impl_aes

  template<int BITS>
  class AesNiImpl : public BlockCipher {
  public:
    AesNiImpl() noexcept
      : BlockCipher(impl_aes::AesAlg<BITS>::value)
    {
    }

    virtual ~AesNiImpl() noexcept
    {
      clear();
    }

    inline size_t blockSize() const
    {
      return Traits::SIZ_BLOCK;
    }

    inline size_t keySize() const
    {
      return Traits::SIZ_KEY;
    }

    inline void clearKey()
    {
      clear();
    }

    inline void setKey(const byte_t *key)
    {
      clear();
      impl_aes::KeyExpansion<Traits,0>::run(_encKeys.data(), key);
      impl_aes::setDecryptKeys<Traits>(_decKeys.data(), _encKeys.data());
    }

    inline void decryptBlock(byte_t *plain, const byte_t *cipher) const
    {
      impl_aes::Decrypt<Traits,0>::run(plain, cipher, _decKeys.data());
    }

    inline void encryptBlock(byte_t *cipher, const byte_t *plain) const
    {
      impl_aes::Encrypt<Traits,0>::run(cipher, plain, _encKeys.data());
    }

  private:
    using S = impl_aes::AESNI;
    using Traits = impl_aes::AesTraits<BITS>;
    using word_t = typename Traits::word_t;
    using KeySchedule = std::array<word_t,Traits::NUM_KEYWORDS>;

    inline void clear()
    {
      _decKeys.fill(0);
      _encKeys.fill(0);
    }

    alignas(S::block_type) KeySchedule _decKeys;
    alignas(S::block_type) KeySchedule _encKeys;
  };

} // namespace cs
