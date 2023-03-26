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
      CRC32 = 0,
      MD5
    };

    enum DigestSize : std::size_t {
      Size_CRC32 = 4,
      Size_MD5 = 16
    };

    Hash(const Function func) noexcept;
    ~Hash() noexcept;

    bool isNull() const;

    std::size_t digestSize() const;
    void reset();
    Buffer result() const;
    void update(const void *data, const std::size_t sizData);

    inline void operator()(const void *data, const std::size_t sizData)
    {
      update(data, sizData);
    }

  private:
    Hash() noexcept = delete;

    HashImplPtr _impl{nullptr};
  };

} // namespace cs
