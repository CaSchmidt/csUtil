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

#include "internal/AesNi128.h"

namespace cs {

  ////// public //////////////////////////////////////////////////////////////

  AesNi128::AesNi128() noexcept
    : BlockCipher(AES128)
  {
  }

  AesNi128::~AesNi128() noexcept
  {
    clear();
  }

  size_t AesNi128::blockSize() const
  {
    return Traits::SIZ_BLOCK;
  }

  size_t AesNi128::keySize() const
  {
    return Traits::SIZ_KEY;
  }

  void AesNi128::clearKey()
  {
    clear();
  }

  void AesNi128::setKey(const byte_t *key)
  {
    clear();
    impl_aes::KeyExpansion<Traits,0>::run(_encKeys.data(), key);
    impl_aes::setDecryptKeys<Traits>(_decKeys.data(), _encKeys.data());
  }

  void AesNi128::decryptBlock(byte_t *plain, const byte_t *cipher) const
  {
    impl_aes::Decrypt<Traits,0>::run(plain, cipher, _decKeys.data());
  }

  void AesNi128::encryptBlock(byte_t *cipher, const byte_t *plain) const
  {
    impl_aes::Encrypt<Traits,0>::run(cipher, plain, _encKeys.data());
  }

  ////// private /////////////////////////////////////////////////////////////

  void AesNi128::clear()
  {
    _decKeys.fill(0);
    _encKeys.fill(0);
  }

} // namespace cs
