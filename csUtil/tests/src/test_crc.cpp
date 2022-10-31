#include <cstdio>
#include <cstdlib>

#include <cs/Core/CRC.h>
#include <cs/IO/File.h>

template<std::size_t SIZE>
void print32(const std::array<uint8_t,SIZE>& buffer)
{
  cs::CRC32 crc32;

  const cs::CRC32::value_type result = crc32(buffer.data(), buffer.size());
  printf("result = 0x%08X\n", result);
}

int main(int argc, char **argv)
{
  /*
  print32(std::array<uint8_t,9>{'1', '2', '3', '4', '5', '6', '7', '8', '9'});
  print32(std::array<uint8_t,4>{0x00, 0x00, 0x00, 0x00});
  print32(std::array<uint8_t,4>{0xFF, 0xFF, 0xFF, 0xFF});
  */

  if( argc - 1 < 1  ||  argv[1] == nullptr ) {
    return EXIT_FAILURE;
  }

  cs::CRC32 crc32;
  cs::CRC32::value_type crcsum = 0;

  cs::File file;
  if( !file.open(argv[1]) ) {
    fprintf(stderr, "ERROR: cs::File::open(\"%s\")\n", argv[1]);
    return EXIT_FAILURE;
  }

  using fsize_type = cs::File::size_type;

  constexpr fsize_type BLOCK = 1024;

  std::array<uint8_t,BLOCK> buffer;

  fsize_type got = 0;
  while( (got = file.read(buffer.data(), buffer.size())) > 0 ) {
    crcsum = crc32(buffer.data(), got, crcsum);
  }

  printf("CRC-32: 0x%08X\n", crcsum);

  return EXIT_SUCCESS;
}
