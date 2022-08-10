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

#include "cs/Trie/FlatTrie.h"

#include "cs/Core/TypeTraits.h"
#include "internal/FlatTrieUtil.h"

namespace cs {

  ////// Private /////////////////////////////////////////////////////////////

  template<typename T>
  class FlatTrieImpl {
  public:
    using     String = std::basic_string<T>;
    using StringList = std::list<String>;

    FlatTrieImpl(const FlatTrie::Letters& letters, const FlatTrie::Links& links,
                 const std::size_t buffer_size = 1024)
      : _buffer_size(buffer_size)
      , _letters(letters)
      , _links(links)
    {
    }

    ~FlatTrieImpl() = default;

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
      if( baseIndex == MAX_SIZE_T ) {
        return words;
      }

      if( impl_trie::isEndOfWord(_links[baseIndex]) ) {
        words.push_back(baseStr);
      }

      const std::size_t destLink = impl_trie::linkIndex(_links[baseIndex]);
      if( destLink > 0 ) {
        String str;
        str.reserve(_buffer_size);
        str = baseStr;
        list(words, str, destLink);
      }

      return words;
    }

    TrieMatch find(const String& str) const
    {
      return isEmpty()  ||  str.empty()
          ? TrieMatch::None
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
    const FlatTrie::Letters& _letters;
    const FlatTrie::Links& _links;

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

        const std::size_t destIndex = impl_trie::linkIndex(_links[i]);
        if( destIndex != 0 ) {
          return base(idxStr + 1, str, destIndex);
        }
      } while( !impl_trie::isEndOfList(_links[i++]) );

      return MAX_SIZE_T;
    }

    TrieMatch find(const std::size_t idxStr, const String& str,
                       const std::size_t baseIndex) const
    {
      std::size_t i = baseIndex;
      do {
        if( _letters[i] != str[idxStr] ) {
          continue;
        }

        if( idxStr + 1 == str.size() ) { // Last character
          if( impl_trie::isEndOfWord(_links[i]) ) {
            return TrieMatch::Exact;
          } else {
            return TrieMatch::Partial;
          }
        }

        const std::size_t destIndex = impl_trie::linkIndex(_links[i]);
        if( destIndex != 0 ) {
          return find(idxStr + 1, str, destIndex);
        }
      } while( !impl_trie::isEndOfList(_links[i++]) );

      return TrieMatch::None;
    }

    void list(StringList& words, String& str,
              const std::size_t baseIndex) const
    {
      std::size_t i = baseIndex;
      do {
        str.push_back(static_cast<T>(_letters[i]));

        if( impl_trie::isEndOfWord(_links[i]) ) {
          words.push_back(str);
        }

        const std::size_t destIndex = impl_trie::linkIndex(_links[i]);
        if( destIndex != 0 ) {
          list(words, str, destIndex);
        }

        str.pop_back();
      } while( !impl_trie::isEndOfList(_links[i++]) );
    }

    FlatTrieImpl() = delete;

    FlatTrieImpl(const FlatTrieImpl&) = delete;
    FlatTrieImpl& operator=(const FlatTrieImpl&) = delete;

    FlatTrieImpl(FlatTrieImpl&&) = delete;
    FlatTrieImpl& operator=(FlatTrieImpl&&) = delete;
  };

  ////// public //////////////////////////////////////////////////////////////

  FlatTrie::FlatTrie(FlatTrie::Links&& links, FlatTrie::Letters&& letters)
    : _letters()
    , _links()
  {
    if( letters.size() == links.size() ) {
      _letters = std::move(letters);
      _links   = std::move(links);
    }
  }

  void FlatTrie::clear()
  {
    _letters.clear();
    _links.clear();
  }

  bool FlatTrie::isEmpty() const
  {
    return _letters.size() != _links.size()  ||  _letters.empty()  ||  _links.empty();
  }

  std::size_t FlatTrie::nodeCount() const
  {
    return isEmpty()
        ? 0
        : _links.size();
  }

  std::size_t FlatTrie::size() const
  {
    return isEmpty()
        ? 0
        : sizeof(letter_type)*_letters.size() + sizeof(link_type)*_links.size();
  }

  // char methods ////////////////////////////////////////////////////////////

  template<>
  std::list<std::string> FlatTrie::complete(const std::string& str) const
  {
    FlatTrieImpl<char> impl(_letters, _links);
    return impl.complete(str);
  }

  template<>
  TrieMatch FlatTrie::find(const std::string& str) const
  {
    FlatTrieImpl<char> impl(_letters, _links);
    return impl.find(str);
  }

  template<>
  std::list<std::string> FlatTrie::list() const
  {
    FlatTrieImpl<char> impl(_letters, _links);
    return impl.list();
  }

  // char16_t methods ////////////////////////////////////////////////////////

  template<>
  std::list<std::u16string> FlatTrie::complete(const std::u16string& str) const
  {
    FlatTrieImpl<char16_t> impl(_letters, _links);
    return impl.complete(str);
  }

  template<>
  TrieMatch FlatTrie::find(const std::u16string& str) const
  {
    FlatTrieImpl<char16_t> impl(_letters, _links);
    return impl.find(str);
  }

  template<>
  std::list<std::u16string> FlatTrie::list() const
  {
    FlatTrieImpl<char16_t> impl(_letters, _links);
    return impl.list();
  }

} // namespace cs
