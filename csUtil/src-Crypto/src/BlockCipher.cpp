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

#include "cs/Crypto/BlockCipher.h"

#include "internal/AesNiImpl.h"

namespace cs {

  ////// public //////////////////////////////////////////////////////////////

  BlockCipher::BlockCipher(const Algorithm id) noexcept
    : _id{id}
  {
  }

  BlockCipher::~BlockCipher() noexcept
  {
  }

  BlockCipher::Algorithm BlockCipher::id() const
  {
    return _id;
  }

  ////// public static ///////////////////////////////////////////////////////

  BlockCipherPtr BlockCipher::make(const Algorithm id)
  {
    if(        id == AES128 ) {
      return std::make_unique<AesNiImpl<128>>();
    } else if( id == AES192 ) {
      return std::make_unique<AesNiImpl<192>>();
    } else if( id == AES256 ) {
      return std::make_unique<AesNiImpl<256>>();
    }
    return BlockCipherPtr();
  }

} // namespace cs
