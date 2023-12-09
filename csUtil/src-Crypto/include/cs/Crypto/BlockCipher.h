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

#include <memory>

#include <cs/Core/csutil_config.h>
#include <cs/Core/TypeTraits.h>

namespace cs {

  using BlockCipherPtr = std::unique_ptr<class BlockCipher>;

  class CS_UTIL_EXPORT BlockCipher {
  public:
    enum Algorithm : unsigned int {
      AES128 = 0,
      AES192,
      AES256
    };

    enum class KeySize : size_t {
      AES128 = 16,
      AES192 = 24,
      AES256 = 32
    };

    enum class BlockSize : size_t {
      AES128 = 16,
      AES192 = 16,
      AES256 = 16
    };

    BlockCipher(const Algorithm id) noexcept;
    virtual ~BlockCipher() noexcept;

    Algorithm id() const;

    virtual size_t blockSize() const = 0;
    virtual size_t keySize() const = 0;

    virtual void clearKey() = 0;
    virtual void setKey(const void *key) = 0;

    virtual void decryptBlock(void *plain, const void *cipher) const = 0;
    virtual void encryptBlock(void *cipher, const void *plain) const = 0;

    static BlockCipherPtr make(const Algorithm id);

  private:
    BlockCipher() noexcept = delete;

    Algorithm _id{};
  };

} // namespace cs
