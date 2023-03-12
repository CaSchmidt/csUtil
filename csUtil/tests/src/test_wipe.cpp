#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include <vector>

#include <cs/Core/ByteArray.h>
#include <cs/Core/Container.h>

bool write(FILE *file, const void *buffer, const std::size_t size)
{
  constexpr std::size_t COUNT = 1;
  return std::fwrite(buffer, size, COUNT, file) == COUNT;
}

int main(int, char **)
{
  constexpr std::size_t BUFFER_SIZE = 1*1024*1024;

  const char *filename = "space.bin";

  cs::ByteArray buffer;
  if( !cs::resize(&buffer, BUFFER_SIZE, '\0') ) {
    fprintf(stderr, "ERROR: resize()\n");
    return EXIT_FAILURE;
  }

  FILE *file = fopen(filename, "wb");
  if( file == NULL ) {
    fprintf(stderr, "ERROR: fopen()\n");
    return EXIT_FAILURE;
  }

  while( write(file, buffer.data(), buffer.size()) ) {
    printf(".");
  }

  fclose(file);

  printf("\nWrote file \"%s\".\n", filename);

  return EXIT_SUCCESS;
}
