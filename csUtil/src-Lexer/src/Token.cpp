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

#include "cs/Lexer/Token.h"

namespace cs {

  ////// BaseToken - public //////////////////////////////////////////////////

  TokenBase::TokenBase(const tokenid_t id, const size_t size,
                       const ctor_tag&) noexcept
    : _id{id}
    , _size{size}
  {
  }

  TokenBase::~TokenBase() noexcept
  {
  }

  tokenid_t TokenBase::id() const
  {
    return _id;
  }

  size_t TokenBase::column() const
  {
    return _column;
  }

  size_t TokenBase::line() const
  {
    return _line;
  }

  void TokenBase::setLocation(const size_t line, const size_t column)
  {
    _column = column;
    _line   = line;
  }

  size_t TokenBase::size() const
  {
    return _size;
  }

  TokenPtr TokenBase::make(const tokenid_t id, const size_t size)
  {
    return std::make_unique<TokenBase>(id, size);
  }

  ////// BaseTokenNames - public /////////////////////////////////////////////

  TokenNamesBase::TokenNamesBase(const ctor_tag&) noexcept
  {
  }

  TokenNamesBase::~TokenNamesBase() noexcept
  {
  }

  const char *TokenNamesBase::name(const tokenid_t id) const
  {
    if(        id == TOK_EndOfInput ) {
      return "EndOfInput";
    } else if( id == TOK_Unknown ) {
      return "Unknown";
    } else if( id == TOK_User ) {
      return "User";
    }
    return nullptr;
  }

  TokenNamesPtr TokenNamesBase::make()
  {
    return std::make_unique<TokenNamesBase>();
  }

} // namespace cs
