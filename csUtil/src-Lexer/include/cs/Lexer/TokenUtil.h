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

#pragma once

#include <limits>

#include <cs/Lexer/ValueToken.h>

namespace cs {

  namespace Token {

    template<typename CharT>
    requires is_tokenid_char_v<CharT>
    constexpr tokenid_t id_from_char(const CharT ch)
    {
      return static_cast<tokenid_t>(ch);
    }

    constexpr bool is_literal(const tokenid_t id)
    {
      return TOK_EndOfInput < id  &&  id < TOK_Unknown;
    }

    template<typename CharT>
    inline bool is_literal(const TokenPtr& token, const CharT ch)
    {
      return token  &&  token->id() == id_from_char(ch);
    }

    template<typename CharT>
    inline TokenPtr make_literal(const CharT ch)
    {
      const tokenid_t id = id_from_char(ch);
      return is_literal(id)
          ? BaseToken::make(id, 1)
          : TokenPtr();
    }

    constexpr tokenid_t make_userid(const tokenid_t user)
    {
      constexpr auto  MAX = std::numeric_limits<tokenid_t>::max();
      constexpr auto USER = static_cast<tokenid_t>(TOK_User);

      return MAX - USER >= user
          ? USER + user
          : 0;
    }

    template<typename T>
    inline T to_value(const TokenPtr& token, const T& defValue = T())
    {
      using Value = ValueToken<T>;

      const Value *ptr = dynamic_cast<const Value*>(token.get());

      return ptr != nullptr
          ? ptr->value()
          : defValue;
    }

  } // namespace Token

} // namespace cs
