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

#include "csUtil/csFlatTrie.h"

#include "csUtil/csTypeTraits.h"
#include "internal/csFlatTrieUtil.h"

////// Private ///////////////////////////////////////////////////////////////

template<typename T>
class csFlatTrieImpl {
public:
  using     String = std::basic_string<T>;
  using StringList = std::list<String>;

  csFlatTrieImpl(const csFlatTrie::Letters& letters, const csFlatTrie::Links& links,
                 const std::size_t buffer_size = 1024)
    : _buffer_size(buffer_size)
    , _letters(letters)
    , _links(links)
  {
  }

  ~csFlatTrieImpl() = default;

  bool isEmpty() const
  {
    return _letters.size() != _links.size()  ||  _letters.empty()  ||  _links.empty();
  }

  StringList complete(const String& baseStr) const
  {
    StringList words;

    if( isEmpty()  ||  baseStr.empty() ) {
      return words;
    }

    const std::size_t baseIndex = base(0, baseStr, 0);
    if( baseIndex == cs::MAX_SIZE_T ) {
      return words;
    }

    if( priv::isEndOfWord(_links[baseIndex]) ) {
      words.push_back(baseStr);
    }

    const std::size_t destLink = priv::linkIndex(_links[baseIndex]);
    if( destLink > 0 ) {
      String str;
      str.reserve(_buffer_size);
      str = baseStr;
      list(words, str, destLink);
    }

    return words;
  }

  cs::TrieMatch find(const String& str) const
  {
    return isEmpty()  ||  str.empty()
        ? cs::TrieMatch::None
        : find(0, str, 0);
  }

  StringList list() const
  {
    StringList words;

    if( isEmpty() ) {
      return words;
    }

    String str;
    str.reserve(_buffer_size);
    list(words, str, 0);

    return words;
  }

private:
  const std::size_t _buffer_size;
  const csFlatTrie::Letters& _letters;
  const csFlatTrie::Links& _links;

  std::size_t base(const std::size_t idxStr, const String& str,
                   const std::size_t baseIndex) const
  {
    std::size_t i = baseIndex;
    do {
      if( _letters[i] != str[idxStr] ) {
        continue;
      }

      if( idxStr + 1 == str.size() ) { // Last character
        return i;
      }

      const std::size_t destIndex = priv::linkIndex(_links[i]);
      if( destIndex != 0 ) {
        return base(idxStr + 1, str, destIndex);
      }
    } while( !priv::isEndOfList(_links[i++]) );

    return cs::MAX_SIZE_T;
  }

  cs::TrieMatch find(const std::size_t idxStr, const String& str,
                     const std::size_t baseIndex) const
  {
    std::size_t i = baseIndex;
    do {
      if( _letters[i] != str[idxStr] ) {
        continue;
      }

      if( idxStr + 1 == str.size() ) { // Last character
        if( priv::isEndOfWord(_links[i]) ) {
          return cs::TrieMatch::Exact;
        } else {
          return cs::TrieMatch::Partial;
        }
      }

      const std::size_t destIndex = priv::linkIndex(_links[i]);
      if( destIndex != 0 ) {
        return find(idxStr + 1, str, destIndex);
      }
    } while( !priv::isEndOfList(_links[i++]) );

    return cs::TrieMatch::None;
  }

  void list(StringList& words, String& str,
            const std::size_t baseIndex) const
  {
    std::size_t i = baseIndex;
    do {
      str.push_back(static_cast<T>(_letters[i]));

      if( priv::isEndOfWord(_links[i]) ) {
        words.push_back(str);
      }

      const std::size_t destIndex = priv::linkIndex(_links[i]);
      if( destIndex != 0 ) {
        list(words, str, destIndex);
      }

      str.pop_back();
    } while( !priv::isEndOfList(_links[i++]) );
  }

  csFlatTrieImpl() = delete;

  csFlatTrieImpl(const csFlatTrieImpl&) = delete;
  csFlatTrieImpl& operator=(const csFlatTrieImpl&) = delete;

  csFlatTrieImpl(csFlatTrieImpl&&) = delete;
  csFlatTrieImpl& operator=(csFlatTrieImpl&&) = delete;
};

////// public ////////////////////////////////////////////////////////////////

csFlatTrie::csFlatTrie(csFlatTrie::Links&& links, csFlatTrie::Letters&& letters)
  : _letters()
  , _links()
{
  if( letters.size() == links.size() ) {
    _letters = std::move(letters);
    _links   = std::move(links);
  }
}

void csFlatTrie::clear()
{
  _letters.clear();
  _links.clear();
}

bool csFlatTrie::isEmpty() const
{
  return _letters.size() != _links.size()  ||  _letters.empty()  ||  _links.empty();
}

std::size_t csFlatTrie::nodeCount() const
{
  return isEmpty()
      ? 0
      : _links.size();
}

std::size_t csFlatTrie::size() const
{
  return isEmpty()
      ? 0
      : sizeof(letter_type)*_letters.size() + sizeof(link_type)*_links.size();
}

// char methods //////////////////////////////////////////////////////////////

template<>
std::list<std::string> csFlatTrie::complete(const std::string& str) const
{
  csFlatTrieImpl<char> impl(_letters, _links);
  return impl.complete(str);
}

template<>
cs::TrieMatch csFlatTrie::find(const std::string& str) const
{
  csFlatTrieImpl<char> impl(_letters, _links);
  return impl.find(str);
}

template<>
std::list<std::string> csFlatTrie::list() const
{
  csFlatTrieImpl<char> impl(_letters, _links);
  return impl.list();
}

// char16_t methods //////////////////////////////////////////////////////////

template<>
std::list<std::u16string> csFlatTrie::complete(const std::u16string& str) const
{
  csFlatTrieImpl<char16_t> impl(_letters, _links);
  return impl.complete(str);
}

template<>
cs::TrieMatch csFlatTrie::find(const std::u16string& str) const
{
  csFlatTrieImpl<char16_t> impl(_letters, _links);
  return impl.find(str);
}

template<>
std::list<std::u16string> csFlatTrie::list() const
{
  csFlatTrieImpl<char16_t> impl(_letters, _links);
  return impl.list();
}
