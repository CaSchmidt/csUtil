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

#include <cs/Core/Buffer.h>

namespace cs {

  class HashImpl;

  using HashImplPtr = std::unique_ptr<HashImpl>;

  class CS_UTIL_EXPORT Hash {
  public:
    enum Function : unsigned int {
      Invalid = 0,
      CRC32,
      MD5,
      SHA1,
      // SHA-2
      SHA224,
      SHA256,
      SHA384,
      SHA512
    };

    enum DigestSize : std::size_t {
      Size_CRC32 = 4,
      Size_MD5 = 16,
      Size_SHA1 = 20,
      // SHA-2
      Size_SHA224 = 28,
      Size_SHA256 = 32,
      Size_SHA384 = 48,
      Size_SHA512 = 64
    };

    Hash(const Function func) noexcept;
    ~Hash() noexcept;

    Function id() const;

    std::size_t digestSize() const;
    void reset();
    Buffer result() const;
    bool update(const void *data, const std::size_t sizData);

    inline bool operator()(const void *data, const std::size_t sizData)
    {
      return update(data, sizData);
    }

  private:
    Hash() noexcept = delete;

    HashImplPtr _impl{nullptr};
  };

} // namespace cs
