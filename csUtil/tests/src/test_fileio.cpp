#include <cstdio>
#include <cstdlib>

#include <csUtil/csFileIO.h>

using StringList = std::list<std::string>;

void print(const StringList& lines)
{
  int no = 0;
  for(const std::string& line : lines) {
#if 0
    printf("HEX:");
    for(std::size_t i = 0; i < line.size(); i++) {
      printf(" %02X", line[i]);
    }
    printf("\n");
#endif
    printf("%2d: <%s>\n", ++no, line.data());
  }
}

int main(int /*argc*/, char **argv)
{
  printf("filename: %s\n", argv[1]);
  const StringList lines = csReadLines(argv[1], true);
  print(lines);

  return EXIT_SUCCESS;
}
