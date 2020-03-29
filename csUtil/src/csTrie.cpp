/****************************************************************************
** Copyright (c) 2018, Carsten Schmidt. All rights reserved.
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

#include "csUtil/csTrie.h"

#include "internal/csTrieNode.h"

////// Private ///////////////////////////////////////////////////////////////

namespace priv {

  template<typename T>
  using String = std::basic_string<T>;

  template<typename T>
  using StringList = std::list<String<T>>;

  template<typename T>
  StringList<T> complete(const csTrieNode *root, const String<T>& needle,
                         const std::size_t buffer_size = 1024)
  {
    if( needle.empty() ) {
      return StringList<T>();
    }

    const csTrieNode *base = root->base(0, needle);
    if( base == nullptr ) {
      return StringList<T>();
    }

    String<T> word;
    word.reserve(buffer_size);
    word = needle;

    StringList<T> words;
    base->list(words, word);

    return words;
  }

  template<typename T>
  cs::TrieMatch find(const csTrieNode *root, const String<T>& needle)
  {
    if( needle.empty() ) {
      return cs::NoMatch;
    }
    return root->find(0, needle);
  }

  template<typename T>
  void insert(csTrieNode *root, const String<T>& s)
  {
    if( s.empty() ) {
      return;
    }
    root->insert(0, s);
  }

  template<typename T>
  void insertReverse(csTrieNode *root, const String<T>& s)
  {
    if( s.empty() ) {
      return;
    }
    root->insertReverse(s.size(), s);
  }

  template<typename T>
  StringList<T> list(const csTrieNode *root, const std::size_t buffer_size = 1024)
  {
    StringList<T> words;

    String<T> word;
    word.reserve(buffer_size);
    root->list(words, word);

    return words;
  }

} // namespace priv

////// public ////////////////////////////////////////////////////////////////

csTrie::csTrie()
  : _root()
{
  _root = std::make_unique<csTrieNode>(0);
}

csTrie::~csTrie() = default;

void csTrie::clear()
{
  _root.reset();
  _root = std::make_unique<csTrieNode>(0);
}

csFlatTrie csTrie::flattened() const
{
  const std::size_t numNodes = nodeCount() - 1; // Don't count 'root'!
  if( numNodes < 1 ) {
    return csFlatTrie();
  }

  csFlatTrie::Links     links(numNodes);
  csFlatTrie::Letters letters(numNodes);

  std::size_t cntNodes = 0;
  _root->flattened(links, letters, cntNodes);

  return csFlatTrie(std::move(links), std::move(letters));
}

bool csTrie::isNull() const
{
  return !_root;
}

std::size_t csTrie::nodeCount() const
{
  std::size_t numNodes = 0;
  _root->statistics(numNodes);

  return numNodes;
}

std::size_t csTrie::size() const
{
  std::size_t numNodes = 0;
  _root->statistics(numNodes);

  return sizeof(csTrieNode)*numNodes;
}

// char methods //////////////////////////////////////////////////////////////

template<>
std::list<std::string> csTrie::complete(const std::string& base) const
{
  return priv::complete(_root.get(), base);
}

template<>
cs::TrieMatch csTrie::find(const std::string& str) const
{
  return priv::find(_root.get(), str);
}

template<>
void csTrie::insert(const std::string& str)
{
  priv::insert(_root.get(), str);
}

template<>
void csTrie::insertReverse(const std::string& str)
{
  priv::insertReverse(_root.get(), str);
}

template<>
std::list<std::string> csTrie::list(char *) const
{
  return priv::list<char>(_root.get());
}

// char16_t methods //////////////////////////////////////////////////////////

template<>
std::list<std::u16string> csTrie::complete(const std::u16string& base) const
{
  return priv::complete(_root.get(), base);
}

template<>
cs::TrieMatch csTrie::find(const std::u16string& str) const
{
  return priv::find(_root.get(), str);
}

template<>
void csTrie::insert(const std::u16string& str)
{
  priv::insert(_root.get(), str);
}

template<>
void csTrie::insertReverse(const std::u16string& str)
{
  priv::insertReverse(_root.get(), str);
}

template<>
std::list<std::u16string> csTrie::list(char16_t *) const
{
  return priv::list<char16_t>(_root.get());
}
