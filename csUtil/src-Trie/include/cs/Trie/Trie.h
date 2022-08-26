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

#pragma once

#include <list>
#include <memory>
#include <string>

#include <cs/Core/csutil_config.h>
#include <cs/Trie/FlatTrie.h>
#include <cs/Trie/TrieMatch.h>

namespace cs {

  class TrieNode;

  class CS_UTIL_EXPORT Trie {
  public:
    Trie();
    ~Trie();

    Trie(Trie&&);
    Trie& operator=(Trie&&);

    void clear();

    FlatTrie flattened() const;

    bool isNull() const;

    std::size_t nodeCount() const;

    std::size_t size() const;

    template<typename T>
    std::list<std::basic_string<T>> complete(const std::basic_string<T>& base) const;

    template<typename T>
    TrieMatch find(const std::basic_string<T>& str) const;

    template<typename T>
    void insert(const std::basic_string<T>& str);

    template<typename T>
    void insertReverse(const std::basic_string<T>& str);

    template<typename T>
    std::list<std::basic_string<T>> list(T * = nullptr) const;

  private:
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

    std::unique_ptr<TrieNode> _root;
  };

  // char methods ////////////////////////////////////////////////////////////

  template<>
  CS_UTIL_EXPORT std::list<std::string> Trie::complete(const std::string& base) const;

  template<>
  CS_UTIL_EXPORT TrieMatch Trie::find(const std::string& str) const;

  template<>
  CS_UTIL_EXPORT void Trie::insert(const std::string& str);

  template<>
  CS_UTIL_EXPORT void Trie::insertReverse(const std::string& str);

  template<>
  CS_UTIL_EXPORT std::list<std::string> Trie::list(char *) const;

  // char16_t methods ////////////////////////////////////////////////////////

  template<>
  CS_UTIL_EXPORT std::list<std::u16string> Trie::complete(const std::u16string& base) const;

  template<>
  CS_UTIL_EXPORT TrieMatch Trie::find(const std::u16string& str) const;

  template<>
  CS_UTIL_EXPORT void Trie::insert(const std::u16string& str);

  template<>
  CS_UTIL_EXPORT void Trie::insertReverse(const std::u16string& str);

  template<>
  CS_UTIL_EXPORT std::list<std::u16string> Trie::list(char16_t *) const;

} // namespace cs
