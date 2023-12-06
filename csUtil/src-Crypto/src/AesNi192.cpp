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

#include "internal/AesNi192.h"

namespace cs {

  ////// public //////////////////////////////////////////////////////////////

  AesNi192::AesNi192() noexcept
    : BlockCipher(AES192)
  {
  }

  AesNi192::~AesNi192() noexcept
  {
    clear();
  }

  size_t AesNi192::blockSize() const
  {
    return Traits::SIZ_BLOCK;
  }

  size_t AesNi192::keySize() const
  {
    return Traits::SIZ_KEY;
  }

  void AesNi192::clearKey()
  {
    clear();
  }

  void AesNi192::setKey(const byte_t *key)
  {
    clear();
    impl_aes::KeyExpansion<Traits,0>::run(_encKeys.data(), key);
    impl_aes::setDecryptKeys<Traits>(_decKeys.data(), _encKeys.data());
  }

  void AesNi192::decryptBlock(byte_t *plain, const byte_t *cipher) const
  {
    using S = impl_aes::AESNI;

    const S::block_type cipher0 = S::load<false>(cipher);

    const S::block_type round0  = S::bit_xor(   cipher0, S::load(&_decKeys[ 0]));
    const S::block_type round1  = S::aesdec(    round0,  S::load(&_decKeys[ 4]));
    const S::block_type round2  = S::aesdec(    round1,  S::load(&_decKeys[ 8]));
    const S::block_type round3  = S::aesdec(    round2,  S::load(&_decKeys[12]));
    const S::block_type round4  = S::aesdec(    round3,  S::load(&_decKeys[16]));
    const S::block_type round5  = S::aesdec(    round4,  S::load(&_decKeys[20]));
    const S::block_type round6  = S::aesdec(    round5,  S::load(&_decKeys[24]));
    const S::block_type round7  = S::aesdec(    round6,  S::load(&_decKeys[28]));
    const S::block_type round8  = S::aesdec(    round7,  S::load(&_decKeys[32]));
    const S::block_type round9  = S::aesdec(    round8,  S::load(&_decKeys[36]));
    const S::block_type round10 = S::aesdec(    round9,  S::load(&_decKeys[40]));
    const S::block_type round11 = S::aesdec(    round10, S::load(&_decKeys[44]));
    const S::block_type round12 = S::aesdeclast(round11, S::load(&_decKeys[48]));

    S::store<false>(plain, round12);
  }

  void AesNi192::encryptBlock(byte_t *cipher, const byte_t *plain) const
  {
    using S = impl_aes::AESNI;

    const S::block_type plain0 = S::load<false>(plain);

    const S::block_type round0  = S::bit_xor(   plain0,  S::load(&_encKeys[ 0]));
    const S::block_type round1  = S::aesenc(    round0,  S::load(&_encKeys[ 4]));
    const S::block_type round2  = S::aesenc(    round1,  S::load(&_encKeys[ 8]));
    const S::block_type round3  = S::aesenc(    round2,  S::load(&_encKeys[12]));
    const S::block_type round4  = S::aesenc(    round3,  S::load(&_encKeys[16]));
    const S::block_type round5  = S::aesenc(    round4,  S::load(&_encKeys[20]));
    const S::block_type round6  = S::aesenc(    round5,  S::load(&_encKeys[24]));
    const S::block_type round7  = S::aesenc(    round6,  S::load(&_encKeys[28]));
    const S::block_type round8  = S::aesenc(    round7,  S::load(&_encKeys[32]));
    const S::block_type round9  = S::aesenc(    round8,  S::load(&_encKeys[36]));
    const S::block_type round10 = S::aesenc(    round9,  S::load(&_encKeys[40]));
    const S::block_type round11 = S::aesenc(    round10, S::load(&_encKeys[44]));
    const S::block_type round12 = S::aesenclast(round11, S::load(&_encKeys[48]));

    S::store<false>(cipher, round12);
  }

  ////// private /////////////////////////////////////////////////////////////

  void AesNi192::clear()
  {
    _decKeys.fill(0);
    _encKeys.fill(0);
  }

} // namespace cs
