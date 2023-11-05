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

#include <md5.h>
#include <sha.h>

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

    virtual Hash::Function id() const = 0;

    virtual void copyResult(void *data) const = 0;
    virtual std::size_t digestSize() const = 0;
    virtual void reset() = 0;
    virtual bool update(const void *data, const std::size_t sizData) = 0;
  };

  /**************************************************************************
   * CRC32 Implementation ***************************************************
   **************************************************************************/

  class HashCrc32 : public HashImpl {
  public:
    HashCrc32() noexcept
      : HashImpl{}
    {
    }

    ~HashCrc32() noexcept
    {
    }

    Hash::Function id() const
    {
      return Hash::CRC32;
    }

    void copyResult(void *data) const
    {
      toBytesBE(data, Hash::Size_CRC32, _sum);
    }

    std::size_t digestSize() const
    {
      return Hash::Size_CRC32;
    }

    void reset()
    {
      _sum = CRC32::makeInit();
    }

    bool update(const void *data, const std::size_t sizData)
    {
      _sum = _crc32(data, sizData, _sum);
      return true;
    }

  private:
    CRC32 _crc32;
    CRC32::value_type _sum{};
  };

  /**************************************************************************
   * MD5 Implementation *****************************************************
   **************************************************************************/

  class HashMd5 : public HashImpl {
  public:
    HashMd5() noexcept
      : HashImpl{}
    {
    }

    ~HashMd5() noexcept
    {
    }

    Hash::Function id() const
    {
      return Hash::MD5;
    }

    void copyResult(void *data) const
    {
      MD5Final(reinterpret_cast<unsigned char*>(data), const_cast<MD5_CTX*>(&_ctx));
    }

    std::size_t digestSize() const
    {
      return Hash::Size_MD5;
    }

    void reset()
    {
      MD5Init(&_ctx);
    }

    bool update(const void *data, const std::size_t sizData)
    {
      MD5Update(&_ctx,
                reinterpret_cast<const unsigned char*>(data),
                static_cast<unsigned int>(sizData));
      return true;
    }

  private:
    MD5_CTX _ctx;
  };

  /**************************************************************************
   * SHA-1 Implementation ***************************************************
   **************************************************************************/

  class HashSha1 : public HashImpl {
  public:
    HashSha1() noexcept
      : HashImpl{}
    {
    }

    ~HashSha1() noexcept
    {
    }

    Hash::Function id() const
    {
      return Hash::SHA1;
    }

    void copyResult(void *data) const
    {
      SHA1Result(const_cast<SHA1Context*>(&_ctx),
                 reinterpret_cast<uint8_t*>(data));
    }

    std::size_t digestSize() const
    {
      return Hash::Size_SHA1;
    }

    void reset()
    {
      SHA1Reset(&_ctx);
    }

    bool update(const void *data, const std::size_t sizData)
    {
      return SHA1Input(&_ctx,
                       reinterpret_cast<const uint8_t*>(data),
                       static_cast<unsigned int>(sizData)) == shaSuccess;
    }

  private:
    SHA1Context _ctx;
  };

  /**************************************************************************
   * SHA-224 (SHA-2) Implementation *****************************************
   **************************************************************************/

  class HashSha2_224 : public HashImpl {
  public:
    HashSha2_224() noexcept
      : HashImpl{}
    {
    }

    ~HashSha2_224() noexcept
    {
    }

    Hash::Function id() const
    {
      return Hash::SHA224;
    }

    void copyResult(void *data) const
    {
      SHA224Result(const_cast<SHA224Context*>(&_ctx),
                   reinterpret_cast<uint8_t*>(data));
    }

    std::size_t digestSize() const
    {
      return Hash::Size_SHA224;
    }

    void reset()
    {
      SHA224Reset(&_ctx);
    }

    bool update(const void *data, const std::size_t sizData)
    {
      return SHA224Input(&_ctx,
                         reinterpret_cast<const uint8_t*>(data),
                         static_cast<unsigned int>(sizData)) == shaSuccess;
    }

  private:
    SHA224Context _ctx;
  };

  /**************************************************************************
   * SHA-256 (SHA-2) Implementation *****************************************
   **************************************************************************/

  class HashSha2_256 : public HashImpl {
  public:
    HashSha2_256() noexcept
      : HashImpl{}
    {
    }

    ~HashSha2_256() noexcept
    {
    }

    Hash::Function id() const
    {
      return Hash::SHA256;
    }

    void copyResult(void *data) const
    {
      SHA256Result(const_cast<SHA256Context*>(&_ctx),
                   reinterpret_cast<uint8_t*>(data));
    }

    std::size_t digestSize() const
    {
      return Hash::Size_SHA256;
    }

    void reset()
    {
      SHA256Reset(&_ctx);
    }

    bool update(const void *data, const std::size_t sizData)
    {
      return SHA256Input(&_ctx,
                         reinterpret_cast<const uint8_t*>(data),
                         static_cast<unsigned int>(sizData)) == shaSuccess;
    }

  private:
    SHA256Context _ctx;
  };

  /**************************************************************************
   * SHA-384 (SHA-2) Implementation *****************************************
   **************************************************************************/

  class HashSha2_384 : public HashImpl {
  public:
    HashSha2_384() noexcept
      : HashImpl{}
    {
    }

    ~HashSha2_384() noexcept
    {
    }

    Hash::Function id() const
    {
      return Hash::SHA384;
    }

    void copyResult(void *data) const
    {
      SHA384Result(const_cast<SHA384Context*>(&_ctx),
                   reinterpret_cast<uint8_t*>(data));
    }

    std::size_t digestSize() const
    {
      return Hash::Size_SHA384;
    }

    void reset()
    {
      SHA384Reset(&_ctx);
    }

    bool update(const void *data, const std::size_t sizData)
    {
      return SHA384Input(&_ctx,
                         reinterpret_cast<const uint8_t*>(data),
                         static_cast<unsigned int>(sizData)) == shaSuccess;
    }

  private:
    SHA384Context _ctx;
  };

  /**************************************************************************
   * SHA-512 (SHA-2) Implementation *****************************************
   **************************************************************************/

  class HashSha2_512 : public HashImpl {
  public:
    HashSha2_512() noexcept
      : HashImpl{}
    {
    }

    ~HashSha2_512() noexcept
    {
    }

    Hash::Function id() const
    {
      return Hash::SHA512;
    }

    void copyResult(void *data) const
    {
      SHA512Result(const_cast<SHA512Context*>(&_ctx),
                   reinterpret_cast<uint8_t*>(data));
    }

    std::size_t digestSize() const
    {
      return Hash::Size_SHA512;
    }

    void reset()
    {
      SHA512Reset(&_ctx);
    }

    bool update(const void *data, const std::size_t sizData)
    {
      return SHA512Input(&_ctx,
                         reinterpret_cast<const uint8_t*>(data),
                         static_cast<unsigned int>(sizData)) == shaSuccess;
    }

  private:
    SHA512Context _ctx;
  };

  /**************************************************************************
   * User Interface *********************************************************
   **************************************************************************/

  ////// public //////////////////////////////////////////////////////////////

  Hash::Hash(const Function func) noexcept
    : _impl{nullptr}
  {
    try {
      if(        func == CRC32 ) {
        _impl = std::make_unique<HashCrc32>();
      } else if( func == MD5 ) {
        _impl = std::make_unique<HashMd5>();
      } else if( func == SHA1 ) {
        _impl = std::make_unique<HashSha1>();
      } else if( func == SHA224 ) {
        _impl = std::make_unique<HashSha2_224>();
      } else if( func == SHA256 ) {
        _impl = std::make_unique<HashSha2_256>();
      } else if( func == SHA384 ) {
        _impl = std::make_unique<HashSha2_384>();
      } else if( func == SHA512 ) {
        _impl = std::make_unique<HashSha2_512>();
      }
    } catch(...) {
      _impl.reset();
      return;
    }

    if( _impl ) {
      _impl->reset();
    }
  }

  Hash::~Hash() noexcept
  {
  }

  bool Hash::isNull() const
  {
    return !_impl;
  }

  Hash::Function Hash::id() const
  {
    return _impl
        ? _impl->id()
        : Invalid;
  }

  std::size_t Hash::digestSize() const
  {
    return _impl->digestSize();
  }

  void Hash::reset()
  {
    _impl->reset();
  }

  Buffer Hash::result() const
  {
    Buffer buf;
    if( !resize(&buf, _impl->digestSize()) ) {
      return Buffer{};
    }

    _impl->copyResult(buf.data());
    _impl->reset();

    return buf;
  }

  bool Hash::update(const void *data, const std::size_t sizData)
  {
    return _impl->update(data, sizData);
  }

} // namespace cs
