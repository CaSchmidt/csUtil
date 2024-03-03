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
#include <siphash.h>

#include "cs/Crypto/Hash.h"

#include "cs/Convert/Serialize.h"
#include "cs/Core/ByteArray.h"
#include "cs/Core/Container.h"
#include "cs/Core/CRC.h"

namespace cs {

  namespace impl_hash {

    template<size_t SIZE>
    using digest_constant = std::integral_constant<size_t,SIZE>;

    template<Hash::Function FUNC>
    struct DigestSize { /* SFINAE */ };
    template<>
    struct DigestSize<Hash::CRC32>           : public digest_constant< 4> {};
    template<>
    struct DigestSize<Hash::MD5>             : public digest_constant<16> {};
    template<>
    struct DigestSize<Hash::SHA1>            : public digest_constant<20> {};
    template<>
    struct DigestSize<Hash::SHA224>          : public digest_constant<28> {};
    template<>
    struct DigestSize<Hash::SHA256>          : public digest_constant<32> {};
    template<>
    struct DigestSize<Hash::SHA384>          : public digest_constant<48> {};
    template<>
    struct DigestSize<Hash::SHA512>          : public digest_constant<64> {};
    template<>
    struct DigestSize<Hash::SipHash_2_4_64>  : public digest_constant< 8> {};
    template<>
    struct DigestSize<Hash::SipHash_2_4_128> : public digest_constant<16> {};

    /************************************************************************
     * CRC32 Implementation *************************************************
     ************************************************************************/

    class HashCrc32 : public Hash {
    public:
      HashCrc32() noexcept
        : Hash(CRC32)
      {
      }

      ~HashCrc32() noexcept
      {
      }

      Buffer digest() const
      {
        Buffer result;
        if( !resize(result, digestSize()) ) {
          return Buffer();
        }

        toBytesBE(result.data(), digestSize(), _sum);
        const_cast<HashCrc32*>(this)->reset();

        return result;
      }

      size_t digestSize() const
      {
        return DigestSize<CRC32>::value;;
      }

      void reset()
      {
        _sum = CRC32::makeInit();
      }

      size_t update(const void *data, const size_t sizData)
      {
        _sum = _crc32(data, sizData, _sum);
        return sizData;
      }

    private:
      ::cs::CRC32 _crc32;
      ::cs::CRC32::value_type _sum{};
    };

    /************************************************************************
     * MD5 Implementation ***************************************************
     ************************************************************************/

    class HashMd5 : public Hash {
    public:
      HashMd5() noexcept
        : Hash(MD5)
      {
      }

      ~HashMd5() noexcept
      {
      }

      Buffer digest() const
      {
        Buffer result;
        if( !resize(result, digestSize()) ) {
          return Buffer();
        }

        MD5Final(reinterpret_cast<unsigned char*>(result.data()),
                 const_cast<MD5_CTX*>(&_ctx));
        const_cast<HashMd5*>(this)->reset();

        return result;
      }

      size_t digestSize() const
      {
        return DigestSize<MD5>::value;
      }

      void reset()
      {
        MD5Init(&_ctx);
      }

      size_t update(const void *data, const size_t sizData)
      {
        constexpr size_t MAX_SIZE = maxab_v<size_t,MD5size>;

        const size_t sizUpdate = std::min(sizData, MAX_SIZE);
        MD5Update(&_ctx,
                  reinterpret_cast<const unsigned char*>(data),
                  static_cast<MD5size>(sizUpdate));

        return sizUpdate;
      }

    private:
      using MD5size = unsigned int;

      MD5_CTX _ctx;
    };

    /************************************************************************
     * SHA-1 Implementation *************************************************
     ************************************************************************/

    class HashSha1 : public Hash {
    public:
      HashSha1() noexcept
        : Hash(SHA1)
      {
      }

      ~HashSha1() noexcept
      {
      }

      Buffer digest() const
      {
        Buffer result;
        if( !resize(result, digestSize()) ) {
          return Buffer();
        }

        SHA1Result(const_cast<SHA1Context*>(&_ctx),
                   reinterpret_cast<uint8_t*>(result.data()));
        const_cast<HashSha1*>(this)->reset();

        return result;
      }

      size_t digestSize() const
      {
        return DigestSize<SHA1>::value;
      }

      void reset()
      {
        SHA1Reset(&_ctx);
      }

      size_t update(const void *data, const size_t sizData)
      {
        constexpr size_t MAX_SIZE = maxab_v<size_t,SHAsize>;

        const size_t sizUpdate = std::min(sizData, MAX_SIZE);
        if( SHA1Input(&_ctx,
                      reinterpret_cast<const uint8_t*>(data),
                      static_cast<SHAsize>(sizUpdate)) != shaSuccess ) {
          return 0;
        }

        return sizUpdate;
      }

    private:
      using SHAsize = unsigned int;

      SHA1Context _ctx;
    };

    /************************************************************************
     * SHA-224 (SHA-2) Implementation ***************************************
     ************************************************************************/

    class HashSha2_224 : public Hash {
    public:
      HashSha2_224() noexcept
        : Hash(SHA224)
      {
      }

      ~HashSha2_224() noexcept
      {
      }

      Buffer digest() const
      {
        Buffer result;
        if( !resize(result, digestSize()) ) {
          return Buffer();
        }

        SHA224Result(const_cast<SHA224Context*>(&_ctx),
                     reinterpret_cast<uint8_t*>(result.data()));
        const_cast<HashSha2_224*>(this)->reset();

        return result;
      }

      size_t digestSize() const
      {
        return DigestSize<SHA224>::value;
      }

      void reset()
      {
        SHA224Reset(&_ctx);
      }

      size_t update(const void *data, const size_t sizData)
      {
        constexpr size_t MAX_SIZE = maxab_v<size_t,SHAsize>;

        const size_t sizUpdate = std::min(sizData, MAX_SIZE);
        if( SHA224Input(&_ctx,
                        reinterpret_cast<const uint8_t*>(data),
                        static_cast<SHAsize>(sizUpdate)) != shaSuccess ) {
          return 0;
        }

        return sizUpdate;
      }

    private:
      using SHAsize = unsigned int;

      SHA224Context _ctx;
    };

    /************************************************************************
     * SHA-256 (SHA-2) Implementation ***************************************
     ************************************************************************/

    class HashSha2_256 : public Hash {
    public:
      HashSha2_256() noexcept
        : Hash(SHA256)
      {
      }

      ~HashSha2_256() noexcept
      {
      }

      Buffer digest() const
      {
        Buffer result;
        if( !resize(result, digestSize()) ) {
          return Buffer();
        }

        SHA256Result(const_cast<SHA256Context*>(&_ctx),
                     reinterpret_cast<uint8_t*>(result.data()));
        const_cast<HashSha2_256*>(this)->reset();

        return result;
      }

      size_t digestSize() const
      {
        return DigestSize<SHA256>::value;
      }

      void reset()
      {
        SHA256Reset(&_ctx);
      }

      size_t update(const void *data, const size_t sizData)
      {
        constexpr size_t MAX_SIZE = maxab_v<size_t,SHAsize>;

        const size_t sizUpdate = std::min(sizData, MAX_SIZE);
        if( SHA256Input(&_ctx,
                        reinterpret_cast<const uint8_t*>(data),
                        static_cast<SHAsize>(sizUpdate)) != shaSuccess ) {
          return 0;
        }

        return sizUpdate;
      }

    private:
      using SHAsize = unsigned int;

      SHA256Context _ctx;
    };

    /************************************************************************
     * SHA-384 (SHA-2) Implementation ***************************************
     ************************************************************************/

    class HashSha2_384 : public Hash {
    public:
      HashSha2_384() noexcept
        : Hash(SHA384)
      {
      }

      ~HashSha2_384() noexcept
      {
      }

      Buffer digest() const
      {
        Buffer result;
        if( !resize(result, digestSize()) ) {
          return Buffer();
        }

        SHA384Result(const_cast<SHA384Context*>(&_ctx),
                     reinterpret_cast<uint8_t*>(result.data()));
        const_cast<HashSha2_384*>(this)->reset();

        return result;
      }

      size_t digestSize() const
      {
        return DigestSize<SHA384>::value;
      }

      void reset()
      {
        SHA384Reset(&_ctx);
      }

      size_t update(const void *data, const size_t sizData)
      {
        constexpr size_t MAX_SIZE = maxab_v<size_t,SHAsize>;

        const size_t sizUpdate = std::min(sizData, MAX_SIZE);
        if( SHA384Input(&_ctx,
                        reinterpret_cast<const uint8_t*>(data),
                        static_cast<SHAsize>(sizUpdate)) != shaSuccess ) {
          return 0;
        }

        return sizUpdate;
      }

    private:
      using SHAsize = unsigned int;

      SHA384Context _ctx;
    };

    /************************************************************************
     * SHA-512 (SHA-2) Implementation ***************************************
     ************************************************************************/

    class HashSha2_512 : public Hash {
    public:
      HashSha2_512() noexcept
        : Hash(SHA512)
      {
      }

      ~HashSha2_512() noexcept
      {
      }

      Buffer digest() const
      {
        Buffer result;
        if( !resize(result, digestSize()) ) {
          return Buffer();
        }

        SHA512Result(const_cast<SHA512Context*>(&_ctx),
                     reinterpret_cast<uint8_t*>(result.data()));
        const_cast<HashSha2_512*>(this)->reset();

        return result;
      }

      size_t digestSize() const
      {
        return DigestSize<SHA512>::value;
      }

      void reset()
      {
        SHA512Reset(&_ctx);
      }

      size_t update(const void *data, const size_t sizData)
      {
        constexpr size_t MAX_SIZE = maxab_v<size_t,SHAsize>;

        const size_t sizUpdate = std::min(sizData, MAX_SIZE);
        if( SHA512Input(&_ctx,
                        reinterpret_cast<const uint8_t*>(data),
                        static_cast<SHAsize>(sizUpdate)) != shaSuccess ) {
          return 0;
        }

        return sizUpdate;
      }

    private:
      using SHAsize = unsigned int;

      SHA512Context _ctx;
    };

    /************************************************************************
     * SipHash-2-4-X ********************************************************
     ************************************************************************/

    template<Hash::Function ID>
    class HashSipHash_2_4_X : public Hash {
    public:
      static_assert( ID == Hash::SipHash_2_4_64  ||  ID == Hash::SipHash_2_4_128 );

      HashSipHash_2_4_X() noexcept
        : Hash(ID)
      {
      }

      ~HashSipHash_2_4_X() noexcept
      {
        clear();
      }

      Buffer digest() const
      {
        const Buffer result(_result.begin(), _result.end());
        const_cast<HashSipHash_2_4_X<ID>*>(this)->reset();

        return result;
      }

      size_t digestSize() const
      {
        return _result.size();
      }

      void reset()
      {
        _result.fill(0);
      }

      size_t update(const void *data, const size_t sizData)
      {
        siphash(data, sizData,
                _key.data(),
                _result.data(), _result.size(),
                2, 4);

        return sizData;
      }

      void clearKey()
      {
        clear();
      }

      size_t keySize() const
      {
        return _key.size();
      }

      bool setKey(const void *key)
      {
        clear();
        for(size_t i = 0; i < _key.size(); i++) {
          _key[i] = reinterpret_cast<const byte_t*>(key)[i];
        }

        return true;
      }

    private:
      static constexpr size_t DIGEST_SIZE = DigestSize<ID>::value;

      inline void clear()
      {
        _key.fill(0);
        _result.fill(0);
      }

      ByteArray<16>          _key;
      ByteArray<DIGEST_SIZE> _result;
    };

  } // namespace impl_hash

  /**************************************************************************
   * User Interface *********************************************************
   **************************************************************************/

  ////// public //////////////////////////////////////////////////////////////

  Hash::Hash(const Function id) noexcept
    : _id{id}
  {
  }

  Hash::~Hash() noexcept
  {
  }

  Hash::Function Hash::id() const
  {
    return _id;
  }

  void Hash::reset()
  {
  }

  void Hash::clearKey()
  {
  }

  size_t Hash::keySize() const
  {
    return 0;
  }

  bool Hash::setKey(const void */*key*/)
  {
    return false;
  }

  ////// public static ///////////////////////////////////////////////////////

  HashPtr Hash::make(const Function id)
  {
    HashPtr result;
    if(        id == CRC32 ) {
      result = std::make_unique<impl_hash::HashCrc32>();
    } else if( id == MD5 ) {
      result = std::make_unique<impl_hash::HashMd5>();
    } else if( id == SHA1 ) {
      result = std::make_unique<impl_hash::HashSha1>();
    } else if( id == SHA224 ) {
      result = std::make_unique<impl_hash::HashSha2_224>();
    } else if( id == SHA256 ) {
      result = std::make_unique<impl_hash::HashSha2_256>();
    } else if( id == SHA384 ) {
      result = std::make_unique<impl_hash::HashSha2_384>();
    } else if( id == SHA512 ) {
      result = std::make_unique<impl_hash::HashSha2_512>();
    } else if( id == SipHash_2_4_64 ) {
      result = std::make_unique<impl_hash::HashSipHash_2_4_X<SipHash_2_4_64>>();
    } else if( id == SipHash_2_4_128 ) {
      result = std::make_unique<impl_hash::HashSipHash_2_4_X<SipHash_2_4_128>>();
    }
    if( result ) {
      result->reset();
    }
    return result;
  }

} // namespace cs
