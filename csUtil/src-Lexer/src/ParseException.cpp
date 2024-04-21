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

    namespace impl_parseex {

      std::ostream& format_id(std::ostream& stream, const tokenid_t id,
                              const BaseTokenNames& names)
      {
        stream << id;

        if(        const char *name = names.name(id);
                   name != nullptr ) {
          stream << " (" << name << ")";
        } else if( const auto ch = Token::id_to_char(id);
                   isPrintableOrSpace(ch) ) {
          stream << " ('" << static_cast<char>(ch) << "')";
        }

        return stream;
      }

      std::ostream& format_id(std::ostream& stream, const TokenPtr& tok,
                              const BaseTokenNames& names)
      {
        return format_id(stream, tok ? tok->id() : TOK_Unknown, names);
      }

    } // namespace impl_parseex

    ////// base //////////////////////////////////////////////////////////////

    base::base(const std::size_t line) noexcept
      : _line{line}
    {
    }

    base::~base() noexcept = default;

    base::base(const base&) noexcept = default;
    base& base::operator=(const base&) noexcept = default;

    std::size_t base::line() const
    {
      return _line;
    }

    ////// unexpected_token //////////////////////////////////////////////////

    unexpected_token::unexpected_token(const tokenid_t expected,
                                       const TokenPtr& unexpected,
                                       const BaseTokenNames& names) noexcept
      : base(unexpected ? unexpected->line() : std::size_t{0})
    {
      std::stringstream stream;

      stream << "expected = ";
      impl_parseex::format_id(stream, expected, names);

      stream << ", ";

      stream << "got = ";
      impl_parseex::format_id(stream, unexpected, names);

      _what = stream.str();
    }

    unexpected_token::~unexpected_token() noexcept = default;

    unexpected_token::unexpected_token(const unexpected_token&) noexcept = default;
    unexpected_token& unexpected_token::operator=(const unexpected_token&) noexcept = default;

    const char *unexpected_token::what() const noexcept
    {
      return _what.data();
    }

  } // namespace parse_exception

} // namespace cs
