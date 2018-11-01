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

#ifndef CSFLATTRIE_H
#define CSFLATTRIE_H

#include <cstdint>

#include <list>
#include <string>
#include <vector>

#include <csUtil/csutil_config.h>
#include <csUtil/csNamespace.h>

class CS_UTIL_EXPORT csFlatTrie {
public:
  using Letters = std::vector<uint16_t>;
  using   Links = std::vector<uint32_t>;

  using letter_type = Letters::value_type;
  using   link_type =   Links::value_type;

  csFlatTrie() = default;
  ~csFlatTrie() = default;

  csFlatTrie(csFlatTrie&&) = default;
  csFlatTrie& operator=(csFlatTrie&&) = default;

  csFlatTrie(Links&& link, Letters&& letters);

  void clear();
  bool isEmpty() const;
  std::size_t nodeCount() const;
  std::size_t size() const;

private:
  csFlatTrie(const csFlatTrie&) = delete;
  csFlatTrie& operator=(const csFlatTrie&) = delete;

  Letters _letters;
  Links   _links;
};

#endif // CSFLATTRIE_H
