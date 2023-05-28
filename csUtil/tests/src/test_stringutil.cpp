#include <cstdio>
#include <cstdlib>

#include <array>

#include <cs/Text/StringUtil.h>

void simplify(const std::string& s)
{
  const std::string x = cs::simplified(s);
  printf("simplify: <%s> -> <%s>\n", s.data(), x.data());
}

void trim(const std::string& s)
{
  const std::string x = cs::trimmed(s);
  printf("trim: <%s> -> <%s>\n", s.data(), x.data());
}

void test_length(const std::array<char,4>& s, const char *name)
{
  printf("length %s = %llu\n", name, cs::strlen(s.data(), s.size()));
}

void test_length()
{
  std::array<char,4> s1{ {'a', 'b', 'c', 'd'} };
  test_length(s1, "s1");
  std::array<char,4> s2{ {'a', 'b', '\0', 'd'} };
  test_length(s2, "s2");
}

void test_ends(const char *hay, const char *needle, const bool ignoreCase)
{
  printf("endsWith(\"%s\", \"%s\", %s) = %s\n",
         hay, needle,
         ignoreCase ? "true" : "false",
         cs::endsWith(hay, cs::MAX_SIZE_T, needle, cs::MAX_SIZE_T, ignoreCase) ? "true" : "false");
}

void test_starts(const char *hay, const char *needle, const bool ignoreCase)
{
  printf("startsWith(\"%s\", \"%s\", %s) = %s\n",
         hay, needle,
         ignoreCase ? "true" : "false",
         cs::startsWith(hay, cs::MAX_SIZE_T, needle, cs::MAX_SIZE_T, ignoreCase) ? "true" : "false");
}

void test_replaceAll(const char *old, const char needle, const char *text)
{
  std::string s(old);
  cs::replaceAll(&s, needle, text);
  printf("replaceAll(\"%s\", '%c', \"%s\") -> \"%s\"\n",
         old, needle, text, s.data());
}

void test_replaceAll(const char *old, const char *needle, const char *text)
{
  std::string s(old);
  cs::replaceAll(&s, needle, cs::MAX_SIZE_T, text);
  printf("replaceAll(\"%s\", \"%s\", \"%s\") -> \"%s\"\n",
         old, needle, text, s.data());
}

int main(int /*argc*/, char ** /*argv*/)
{
  const std::string null;
  const std::string empty("");
  const std::string letter1("l");
  const std::string letter2(" l ");
  const std::string single("single");
  const std::string simple("   simple   ");
  const std::string example("  lots\t of\nwhitespace\r\n ");

  simplify(null);
  simplify(empty);
  simplify(letter1);
  simplify(letter2);
  simplify(single);
  simplify(simple);
  simplify(example);

  printf("\n---------\n\n");

  trim(null);
  trim(empty);
  trim(letter1);
  trim(letter2);
  trim(single);
  trim(simple);
  trim(example);

  printf("\n---------\n\n");

  test_length();

  printf("\n---------\n\n");

  test_ends("abcxyz", "xyz", false);
  test_ends("abcxyz", "xyz", true);
  test_ends("abcxyz", "XYZ", false);
  test_ends("abcxyz", "XYZ", true);

  printf("\n---------\n\n");

  test_starts("abcxyz", "abc", false);
  test_starts("abcxyz", "abc", true);
  test_starts("abcxyz", "ABC", false);
  test_starts("abcxyz", "ABC", true);

  printf("\n---------\n\n");

  test_replaceAll("xyzxyz", 'x', "a.");
  test_replaceAll("xyzxyz", 'y', ".b.");
  test_replaceAll("xyzxyz", 'z', ".c");
  test_replaceAll("xyzxyz", 'X', "a.");

  printf("\n---------\n\n");

  test_replaceAll("xyzxyz", "xy", "ab.");
  test_replaceAll("xyzxyz", "y", ".b.");
  test_replaceAll("xyzxyz", "yz", ".bc");
  test_replaceAll("xyzxyz", "abc", "abc.");
  test_replaceAll("xyzxyz", "xyz", "abc");
  test_replaceAll("xyzxyz", "xyzxyz", "abc");
  test_replaceAll("xyzxyz", "xyzxyzxyz", "abc");

  return EXIT_SUCCESS;
}
