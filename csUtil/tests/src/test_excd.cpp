#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include <algorithm>
#include <array>
#include <vector>

#include <cs/Core/Container.h>
#include <cs/IO/File.h>

namespace cdrom {

  constexpr std::size_t SIZ_Sect  = 2352; // Audio CD Sector Size
  constexpr std::size_t SIZ_Sync  =   12;
  constexpr std::size_t SIZ_Addr  =    3;
  constexpr std::size_t SIZ_Mode  =    1;
  constexpr std::size_t SIZ_Data1 = 2048; // Mode 1 Data
  constexpr std::size_t SIZ_Data2 = 2336; // Mode 2 Data

  constexpr std::size_t OFF_Sync = 0;
  constexpr std::size_t OFF_Addr = OFF_Sync + SIZ_Sync;
  constexpr std::size_t OFF_Mode = OFF_Addr + SIZ_Addr;
  constexpr std::size_t OFF_Data = OFF_Mode + SIZ_Mode;

  const std::array<uint8_t,SIZ_Sync> sync_pattern{
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
  };

  constexpr uint8_t INVALID_MODE = 0;

  constexpr std::size_t SIZE_READBUFFER = SIZ_Sect*8*1024;

} // namespace cdrom

uint8_t scanBuffer(const cs::ByteArray& buffer, const std::size_t have)
{
  if( have < 1 ) {
    return cdrom::INVALID_MODE;
  }

  uint8_t result = cdrom::INVALID_MODE;

  const std::size_t numSector = have/cdrom::SIZ_Sect;
  const std::size_t numRemain = have%cdrom::SIZ_Sect;

  for(std::size_t i = 0; i < numSector; i++) {
    const uint8_t *sector = buffer.data() + i*cdrom::SIZ_Sect;
    const uint8_t    mode = sector[cdrom::OFF_Mode];

    if( !std::equal(cdrom::sync_pattern.begin(), cdrom::sync_pattern.end(), sector) ) {
      return cdrom::INVALID_MODE;
    }

    if(        result == cdrom::INVALID_MODE ) {
      result = mode;
    } else if( result != mode ) {
      return cdrom::INVALID_MODE;
    }
  }

  if( numRemain > 0 ) {
    fprintf(stderr, "WARNING: %d byte(s) of junk detected!\n", int(numRemain));
  }

  return result;
}

void writeBuffer(const cs::File& out, const cs::ByteArray& buffer, const std::size_t have,
                 const std::size_t data_size)
{
  if( have < 1  ||  data_size < 1  ||  data_size > cdrom::SIZ_Data2 ) {
    return;
  }

  const std::size_t numSector = have/cdrom::SIZ_Sect;
  const std::size_t numRemain = have%cdrom::SIZ_Sect;

  for(std::size_t i = 0; i < numSector; i++) {
    const uint8_t *sector = buffer.data() + i*cdrom::SIZ_Sect;
    const uint8_t   *data = sector + cdrom::OFF_Data;

    out.write(data, data_size);
  }

  if( numRemain > 0 ) {
    fprintf(stderr, "WARNING: %d byte(s) of junk detected!\n", int(numRemain));
  }
}

int main(int /*argc*/, char **argv)
{
  const char *inname = argv[1];
  cs::File infile;
  if( !infile.open(cs::UTF8(inname)) ) {
    fprintf(stderr, "ERROR: Unable to open file \"%s\"!\n", inname);
    return EXIT_FAILURE;
  }

  printf("File \"%s\" opened.\n", cs::CSTR(infile.path().u8string().data()));

  cs::ByteArray buffer;
  std::size_t     have;
  uint8_t         mode = cdrom::INVALID_MODE;

  if( !cs::resize(&buffer, cdrom::SIZE_READBUFFER) ) {
    fprintf(stderr, "ERROR: Unable to allocate read buffer!\n");
    return EXIT_FAILURE;
  }

  // (1) Scan ////////////////////////////////////////////////////////////////

  infile.seek(0);
  while( (have = infile.read(buffer.data(), buffer.size())) > 0 ) {
    const uint8_t m = scanBuffer(buffer, have);

    if(        m == cdrom::INVALID_MODE ) {
      fprintf(stderr, "ERROR: Unable to detect mode!\n");
      return EXIT_FAILURE;
    } else if( mode == cdrom::INVALID_MODE ) {
      mode = m;
    } else if( mode != m ) {
      fprintf(stderr, "ERROR: Mixed-Mode detected!\n");
      return EXIT_FAILURE;
    }
  }

  std::size_t data_size = 0;
  if(        mode == 1 ) {
    data_size = cdrom::SIZ_Data1;
  } else if( mode == 2 ) {
    data_size = cdrom::SIZ_Data2;
  } else {
    fprintf(stderr, "ERROR: Invalid mode detected!\n");
    return EXIT_FAILURE;
  }

  printf("Mode %d detected (%d bytes/sector).\n", int(mode), int(data_size));

  // (2) Write ///////////////////////////////////////////////////////////////

  const char *outname = "output.iso";
  cs::File outfile;
  if( !outfile.open(outname, cs::FileOpenFlag::Write) ) {
    fprintf(stderr, "ERROR: Unable to open file \"%s\" for writing!\n", outname);
    return EXIT_FAILURE;
  }

  infile.seek(0);
  while( (have = infile.read(buffer.data(), buffer.size())) > 0 ) {
    writeBuffer(outfile, buffer, have, data_size);
  }

  return EXIT_SUCCESS;
}
