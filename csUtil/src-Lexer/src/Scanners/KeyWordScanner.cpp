/****************************************************************************
** Copyright (c) 2024, Carsten Schmidt. All rights reserved.
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

#include "cs/Lexer/Scanners/KeyWordScanner.h"

namespace cs {

  ////// public //////////////////////////////////////////////////////////////

  KeyWordScanner::KeyWordScanner(const ctor_tag&) noexcept
  {
  }

  KeyWordScanner::~KeyWordScanner() noexcept
  {
  }

  bool KeyWordScanner::addWord(KeyWord word)
  {
    if( word.first < TOK_User  ||  word.second.empty() ) {
      return false;
    }

    _words.push_back(std::move(word));

    return true;
  }

  TokenPtr KeyWordScanner::scan(const StringView& input) const
  {
    for(const KeyWord& word : _words) {
      // TODO: Check, if succeeding character is not an identifier?
      if( input.starts_with(word.second) ) {
        return TokenBase::make(word.first, word.second.size());
      }
    }

    return TokenPtr();
  }

  ScannerPtr KeyWordScanner::make()
  {
    return std::make_unique<KeyWordScanner>();
  }

} // namespace cs
