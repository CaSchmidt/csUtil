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

#include "cs/Crypto/Hash.h"

#include "cs/Convert/Serialize.h"
#include "cs/Core/Container.h"
#include "cs/Core/CRC.h"

namespace cs {

  /**************************************************************************
   * Implementation Interface ***********************************************
   **************************************************************************/

  class HashImpl {
  public:
    HashImpl() noexcept
    {
    }

    virtual ~HashImpl() noexcept
    {
    }

    virtual void reset() = 0;
    virtual Buffer result() const = 0;
    virtual std::size_t size() const = 0;
    virtual void update(const void *data, const std::size_t sizData) = 0;
  };

  /**************************************************************************
   * CRC32 Implementation ***************************************************
   **************************************************************************/

  class HashCrc32 : public HashImpl {
  public:
    HashCrc32() noexcept
      : HashImpl()
    {
    }

    ~HashCrc32() noexcept
    {
    }

    void reset()
    {
      _sum = CRC32::makeInit();
    }

    Buffer result() const
    {
      Buffer buf;
      if( !resize(&buf, size()) ) {
        return Buffer{};
      }

      toBytesBE(buf.data(), buf.size(), _sum);

      return buf;
    }

    std::size_t size() const
    {
      return 4;
    }

    void update(const void *data, const std::size_t sizData)
    {
      _sum = _crc32(data, sizData, _sum);
    }

  private:
    CRC32 _crc32;
    CRC32::value_type _sum{};
  };

  /**************************************************************************
   * User Interface *********************************************************
   **************************************************************************/

  ////// public //////////////////////////////////////////////////////////////

  Hash::Hash(const Algorithm algo) noexcept
    : _impl{nullptr}
  {
    try {
      if( algo == CRC32 ) {
        _impl = std::make_unique<HashCrc32>();
      }
    } catch(...) {
      _impl.reset();
      return;
    }

    _impl->reset();
  }

  Hash::~Hash() noexcept
  {
  }

  bool Hash::isNull() const
  {
    return !_impl;
  }

  void Hash::reset()
  {
    _impl->reset();
  }

  Buffer Hash::result() const
  {
    return _impl->result();
  }

  std::size_t Hash::size() const
  {
    return _impl->size();
  }

  void Hash::update(const void *data, const std::size_t sizData)
  {
    _impl->update(data, sizData);
  }

} // namespace cs
