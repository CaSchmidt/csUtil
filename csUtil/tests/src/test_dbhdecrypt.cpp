#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <filesystem>
#include <format>
#include <list>
#include <print>

#include <cs/Convert/BufferUtil.h>
#include <cs/Core/CharUtil.h>
#include <cs/Core/Endian.h>
#include <cs/Crypto/BlockCipher.h>
#include <cs/Crypto/Hash.h>
#include <cs/IO/File.h>
#include <cs/Math/Numeric.h>
#include <cs/System/FileSystem.h>
#include <cs/System/PathFormatter.h>

#pragma warning (disable : 4996)

namespace chr = std::chrono;
namespace  fs = std::filesystem;

struct PlainInfo {
  PlainInfo(const std::string& key = std::string(),
            const std::string& plain = std::string()) noexcept
    : keystr(key)
    , plainstr(plain)
  {
    if( plainstr.empty() ) {
      return;
    }
    numSpaces = std::count(plainstr.begin(), plainstr.end(), ' ');
#if 0
    numSpaces += numLeadingSpace();
#endif
  }

  bool isEmpty() const
  {
    return keystr.empty()  ||  plainstr.empty();
  }

  std::size_t numLeadingSpace() const
  {
    std::size_t count{0};
    for(std::size_t i = 0; i < plainstr.size(); i++) {
      if( plainstr[i] != ' ' ) {
        break;
      } else {
        count++;
      }
    }
    return count;
  }

  int i{0};
  std::string keystr;
  std::size_t numSpaces{0};
  std::string plainstr;
};

inline bool operator<(const PlainInfo& a, const PlainInfo& b)
{
  return a.numSpaces < b.numSpaces;
}

PlainInfo decrypt(const std::string& keystr, const cs::Buffer& cipher)
{
  constexpr std::size_t ONE = 1;

  cs::BlockCipherPtr aes128 = cs::BlockCipher::make(cs::BlockCipher::AES128);
  if( !aes128 ) {
    return PlainInfo();
  }
  const std::size_t sizBlock = aes128->blockSize();

  const cs::Buffer key = cs::fromHexString(keystr).value();
  if( key.size() != aes128->keySize() ) {
    return PlainInfo();
  }
  aes128->setKey(key.data());

  const std::size_t numBlocks = cipher.size()/sizBlock;
  if( numBlocks < ONE ) {
    return PlainInfo();
  }

  std::string plainstr(numBlocks*sizBlock, ' ');
  for(std::size_t i = 0; i < numBlocks; i++) {
    char              *plainptr = plainstr.data() + i*sizBlock;
    const cs::byte_t *cipherptr = cipher.data()   + i*sizBlock;
    aes128->decryptBlock(plainptr, cipherptr);
  }

  std::replace_if(plainstr.begin(), plainstr.end(),
                  [](const char& ch) -> bool {
    return !cs::isPrintableOrSpace(ch);
  }, ' ');

  return PlainInfo(keystr, plainstr);
}

std::string fileModificationTime(const fs::path& p)
{
  const chr::system_clock::time_point filtim_sys = cs::lastWriteTime(p);
  const auto filtim_loc = chr::current_zone()->to_local(filtim_sys);
  std::string s = std::format("{0:%d}-{0:%m}-{0:%Y} {0:%H}:{0:%M}:{0:%S}", filtim_loc);
  const std::size_t posDot = s.rfind('.');
  if( posDot != std::string::npos ) {
    s.resize(posDot);
  }
  return s;
}

std::string md5hexhash(const std::string& s)
{
  cs::HashPtr md5 = cs::Hash::make(cs::Hash::MD5);

  md5->update(s.data(), s.size());
  const cs::Buffer digest = md5->digest();

  return cs::toHexString(digest).value();
}

std::string mixhash(const std::string& first, const std::string& second)
{
  constexpr std::size_t SIZ_MD5HEXHASH = 32;

  if( first.size() != SIZ_MD5HEXHASH  ||  second.size() != SIZ_MD5HEXHASH ) {
    return std::string();
  }

  std::string result(SIZ_MD5HEXHASH, ' ');

  for(std::size_t i = 0; i < SIZ_MD5HEXHASH; i++) {
    result[i] = cs::isEven(i)
        ? first.at(i)
        : second.at(i);
  }

  return result;
}

int main(int argc, char **argv)
{
  if( argc - 1 < 1 ) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return EXIT_FAILURE;
  }

  const fs::path input(argv[1]);
  const std::string mtime = fileModificationTime(input);

  cs::File file;
  if( !file.open(input) ) {
    fprintf(stderr, "ERROR: File::open()\n");
    return EXIT_FAILURE;
  }
  const cs::Buffer cipher = file.readAll();
  file.close();
  if( cipher.empty() ) {
    fprintf(stderr, "ERROR: File::readAll()\n");
    return EXIT_FAILURE;
  }

  const std::string digest = md5hexhash(mtime);
  std::println("{}", input.filename());
  std::println("MD5(\"{}\"): {}", mtime, digest);
  std::println("");

  std::list<PlainInfo> infos;
  for(int i = 1; i <= 10000; i++) { // cf. dbh_crypter.exe
    const std::string s = std::format("{}", i);
    const std::string keystr = mixhash(md5hexhash(s), digest);

    // cs::println("%: %", s, keystr);
    const PlainInfo info = decrypt(keystr, cipher);
    if( info.isEmpty()              ||
        info.numLeadingSpace() > 0  ||
        info.numSpaces > info.plainstr.size()/2 ) {
      continue;
    } else {
      infos.push_back(info);
    }
  }
  infos.sort();

  for(const PlainInfo& info : infos) {
    std::println("<{}>", info.plainstr);
  }

  return EXIT_SUCCESS;
}
