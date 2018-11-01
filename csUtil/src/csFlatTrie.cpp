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

#include "internal/csFlatTrieUtil.h"

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
