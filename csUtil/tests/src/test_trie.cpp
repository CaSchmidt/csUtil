#include <cstdio>
#include <cstdlib>

#include <regex>
#include <set>

#include <csUtil/csFileIO.h>
#include <csUtil/csFlatTrie.h>
#include <csUtil/csTrie.h>
#include <csUtil/csTypeTraits.h>

#undef HAVE_REVERSE

template<typename T>
inline T reversed(const T& in)
{
  T out;
  if( in.empty() ) {
    return out;
  }
  out.resize(in.size());
  for(std::size_t i = 0; i < in.size(); i++) {
    out[i] = in[in.size() - 1 - i];
  }
  return out;
}

typedef std::set<std::string> Words;

Words readWords(const std::u8string& filename)
{
  Words words;

  bool ok;
  const std::string content = csReadTextFile(filename, &ok);
  if( !ok ) {
    fprintf(stderr, "ERROR: Unable to open file \"%s\"!\n",
            cs::CSTR(filename.data()));
    return words;
  }

  const std::regex rx("[_0-9a-zA-Z]+");
  std::smatch results;
  std::string text = content;
  while( std::regex_search(text, results, rx) ) {
    words.insert(results.str(0));
    text = results.suffix();
  }

  return words;
}

template<typename T>
void complete(const T& trie, const char *s)
{
  printf("%s...\n", s);
  const std::list<std::string> result = trie.complete(std::string(s));
  for(const std::string& s : result) {
    printf("...%s\n", s.data());
  }
}

int main(int /*argc*/, char **argv)
{
  const Words words = readWords(cs::UTF8(argv[1]));

  csTrie trie;
  for(const std::string& w : words) {
#ifndef HAVE_REVERSE
    trie.insert(w);
#else
    trie.insertReverse(w);
#endif
  }

  csFlatTrie flatTrie = trie.flattened();

  std::size_t cntTrie(0), cntFlatTrie(0);
  for(const std::string& w : words) {
#ifndef HAVE_REVERSE
    if( trie.find(w) == cs::ExactMatch ) {
      cntTrie++;
    }
    if( flatTrie.find(w) == cs::ExactMatch ) {
      cntFlatTrie++;
    }
#else
    if( trie.find(reversed(w)) == cs::ExactMatch ) {
      cntTrie++;
    }
    if( flatTrie.find(reversed(w)) == cs::ExactMatch ) {
      cntFlatTrie++;
    }
#endif
  }

  printf("***");
#ifdef HAVE_REVERSE
  printf(" REVERSE");
#endif
  printf(" FIND\n");
  printf("trie: %llu of %llu\n", cntTrie, words.size());
  printf("flat: %llu of %llu\n", cntFlatTrie, words.size());

#ifndef HAVE_REVERSE
  printf("*** COMPLETE\n");
  complete(flatTrie, "s");
  complete(flatTrie, "you");
  complete(flatTrie, "Do");
#endif

  return EXIT_SUCCESS;
}
