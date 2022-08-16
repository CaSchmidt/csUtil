#include <cstdio>
#include <cstdlib>

#include <cs/IO/FileIO.h>
#include <cs/Unicode/TextConverter.h>

void print(const std::string& s)
{
  for(std::string::size_type i = 0; i < s.size(); i++) {
    printf("%2llu: 0x%02X\n", i, static_cast<uint8_t>(s[i]));
  }
}

void print(const std::u16string& s)
{
  for(std::u16string::size_type i = 0; i < s.size(); i++) {
    printf("%2llu: 0x%04X\n", i, static_cast<uint16_t>(s[i]));
  }
}

void print_sep()
{
  printf("---\n");
}

int main(int /*argc*/, char ** /*argv*/)
{
  const cs::TextConverter c_ascii = cs::TextConverter::createAscii();
  const cs::TextConverter    c_l1 = cs::TextConverter::createLatin1();
  const cs::TextConverter    c_l9 = cs::TextConverter::createLatin9();
  const cs::TextConverter  c_utf8 = cs::TextConverter::createUtf8();
  const cs::TextConverter  c_1252 = cs::TextConverter::createWindows1252();

  const std::string utf8("für€");
  print(utf8); print_sep();

  const std::u16string utf16 = c_utf8.toUnicode(utf8);
  print(utf16); print_sep();

  const std::string ebit = c_1252.fromUnicode(utf16);
  print(ebit); print_sep();

  printf("*** File I/O\n");

  const std::string fileutf8 = cs::readTextFile(cs::UTF8("../../csUtil/csUtil/testutils/data/test äuml.txt"));
  print(fileutf8); print_sep();

  return EXIT_SUCCESS;
}
