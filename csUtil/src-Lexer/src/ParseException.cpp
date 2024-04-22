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

#include <sstream>

#include "cs/Lexer/ParseException.h"

#include "cs/Core/CharUtil.h"
#include "cs/Lexer/TokenUtil.h"

namespace cs {

  namespace parse_exception {

    namespace impl_parser {

      inline std::size_t getTokenLine(const TokenPtr& token)
      {
        return token
            ? token->line()
            : std::size_t{0};
      }

      inline const char *getTokenName(const TokenNamesPtr& names, const tokenid_t id)
      {
        return names
            ? names->name(id)
            : nullptr;
      }

      constexpr auto char_from_id(const tokenid_t id)
      {
        return static_cast<CharacterOfSize<sizeof(tokenid_t)>::char_type>(id);
      }

      std::ostream& format_id(std::ostream& stream, const tokenid_t id,
                              const TokenNamesPtr& names)
      {
        stream << id;

        if(        const char *name = getTokenName(names, id);
                   name != nullptr ) {
          stream << " (" << name << ")";
        } else if( const auto ch = char_from_id(id);
                   isPrintableOrSpace(ch) ) {
          stream << " ('" << static_cast<char>(ch) << "')";
        }

        return stream;
      }

      std::ostream& format_id(std::ostream& stream, const TokenPtr& token,
                              const TokenNamesPtr& names)
      {
        return format_id(stream, token ? token->id() : TOK_Unknown, names);
      }

      std::ostream& format_value(std::ostream& stream, const TokenPtr& token)
      {
        if( const auto *ptr = dynamic_cast<const ValueToken<std::string>*>(token.get()) ;
            ptr != nullptr ) {
          stream << "\"" << ptr->value() << "\"";
        } else {
          stream << "UNKNOWN VALUE";
        }

        return stream;
      }

    } // namespace impl_parser

    ////// base //////////////////////////////////////////////////////////////

    base_exception::base_exception(const std::size_t line) noexcept
      : _line{line}
    {
    }

    base_exception::~base_exception() noexcept = default;

    base_exception::base_exception(const base_exception&) noexcept = default;
    base_exception& base_exception::operator=(const base_exception&) noexcept = default;

    std::size_t base_exception::line() const
    {
      return _line;
    }

    ////// unexpected_token //////////////////////////////////////////////////

    unexpected_token::unexpected_token(const TokenPtr& token,
                                       const TokenNamesPtr& names) noexcept
      : base_exception(impl_parser::getTokenLine(token))
    {
      std::stringstream stream;

      stream << "unexpected token = ";
      impl_parser::format_id(stream, token, names);

      _what = stream.str();
    }

    unexpected_token::unexpected_token(const tokenid_t expected,
                                       const TokenPtr& unexpected,
                                       const TokenNamesPtr& names) noexcept
      : base_exception(impl_parser::getTokenLine(unexpected))
    {
      std::stringstream stream;

      stream << "expected token = ";
      impl_parser::format_id(stream, expected, names);

      stream << ", ";

      stream << "have token = ";
      impl_parser::format_id(stream, unexpected, names);

      _what = stream.str();
    }

    unexpected_token::~unexpected_token() noexcept = default;

    unexpected_token::unexpected_token(const unexpected_token&) noexcept = default;
    unexpected_token& unexpected_token::operator=(const unexpected_token&) noexcept = default;

    const char *unexpected_token::what() const noexcept
    {
      return _what.data();
    }

    ////// unexpected_token_value ////////////////////////////////////////////

    unexpected_token_value::unexpected_token_value(const TokenPtr& token,
                                                   const TokenNamesPtr& names) noexcept
      : base_exception(impl_parser::getTokenLine(token))
    {
      std::stringstream stream;

      stream << "unexpected value = " ;
      impl_parser::format_value(stream, token);

      stream << ", ";

      stream << "token = ";
      impl_parser::format_id(stream, token, names);

      _what = stream.str();
    }

    unexpected_token_value::~unexpected_token_value() noexcept = default;

    unexpected_token_value::unexpected_token_value(const unexpected_token_value&) noexcept = default;
    unexpected_token_value& unexpected_token_value::operator=(const unexpected_token_value&) noexcept = default;

    const char *unexpected_token_value::what() const noexcept
    {
      return _what.data();
    }

  } // namespace parse_exception

} // namespace cs
