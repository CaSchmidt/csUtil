#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include <vector>

bool write(FILE *file, const void *buffer, const std::size_t size)
{
  constexpr std::size_t COUNT = 1;
  return std::fwrite(buffer, size, COUNT, file) == COUNT;
}

int main(int, char **)
{
  constexpr std::size_t BUFFER_SIZE = 1*1024*1024;

  const char *filename = "space.bin";

  std::vector<uint8_t> buffer;
  try {
    buffer.resize(BUFFER_SIZE, '\0');
  } catch(...) {
    buffer.clear();
    fprintf(stderr, "ERROR: std::vector<>::resize()\n");
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
