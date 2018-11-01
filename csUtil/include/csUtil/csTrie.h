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

#ifndef CSTRIE_H
#define CSTRIE_H

#include <list>
#include <memory>
#include <string>

#include <csUtil/csutil_config.h>
#include <csUtil/csFlatTrie.h>
#include <csUtil/csNamespace.h>

class csTrieNode;

class CS_UTIL_EXPORT csTrie {
public:
  csTrie();
  ~csTrie();

  void clear();

  csFlatTrie flattened() const;

  bool isNull() const;

  std::size_t nodeCount() const;

  std::size_t size() const;

  template<typename T>
  std::list<std::basic_string<T>> complete(const std::basic_string<T>& base) const;

  template<typename T>
  cs::TrieMatch find(const std::basic_string<T>& str) const;

  template<typename T>
  void insert(const std::basic_string<T>& str);

  template<typename T>
  void insertReverse(const std::basic_string<T>& str);

  template<typename T>
  std::list<std::basic_string<T>> list(T * = nullptr) const;

private:
  csTrie(const csTrie&) = delete;
  csTrie& operator=(const csTrie&) = delete;

  std::unique_ptr<csTrieNode> _root;
};

// char methods //////////////////////////////////////////////////////////////

template<>
CS_UTIL_EXPORT std::list<std::string> csTrie::complete(const std::string& base) const;

template<>
CS_UTIL_EXPORT cs::TrieMatch csTrie::find(const std::string& str) const;

template<>
CS_UTIL_EXPORT void csTrie::insert(const std::string& str);

template<>
CS_UTIL_EXPORT void csTrie::insertReverse(const std::string& str);

template<>
CS_UTIL_EXPORT std::list<std::string> csTrie::list(char *) const;

// char16_t methods //////////////////////////////////////////////////////////

template<>
CS_UTIL_EXPORT std::list<std::u16string> csTrie::complete(const std::u16string& base) const;

template<>
CS_UTIL_EXPORT cs::TrieMatch csTrie::find(const std::u16string& str) const;

template<>
CS_UTIL_EXPORT void csTrie::insert(const std::u16string& str);

template<>
CS_UTIL_EXPORT void csTrie::insertReverse(const std::u16string& str);

template<>
CS_UTIL_EXPORT std::list<std::u16string> csTrie::list(char16_t *) const;

#endif // CSTRIE_H
