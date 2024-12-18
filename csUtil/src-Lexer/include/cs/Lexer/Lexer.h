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

#include <list>
#include <string>

#include <cs/Core/Flags.h>
#include <cs/Lexer/Scanner.h>
#include <cs/Lexer/ValueToken.h>

namespace cs {

  enum class LexerFlag : unsigned {
    None   = 0,
    ScanHT = 0x01,
    ScanLF = 0x02,
    All    = 0xFF
  };

} // namespace cs

CS_ENABLE_FLAGS(cs::LexerFlag)

namespace cs {

  using LexerFlags = Flags<LexerFlag>;

  class CS_UTIL_EXPORT Lexer {
  public:
    using  char_type = IScanner::char_type;
    using  size_type = IScanner::size_type;
    using String     = std::basic_string<char_type>;
    using StringView = IScanner::StringView;

    using Scanners = std::list<ScannerPtr>;

    Lexer() noexcept;
    ~Lexer() noexcept;

    bool addScanner(ScannerPtr scanner);
    void clear();
    void initialize(String input);
    TokenPtr nextToken();
    void setFlags(const LexerFlags flags);

  private:
    Lexer(const Lexer&) = delete;
    Lexer& operator=(const Lexer&) = delete;

    static_assert( std::is_unsigned_v<size_type> );

    static constexpr std::size_t FIRST_COLUMN = 1;
    static constexpr std::size_t FIRST_LINE   = 1;

    void advance(const char_type ch);
    bool atEnd() const;
    void consume(const size_type _len);
    size_type remaining() const;
    TokenPtr skip();
    StringView view() const;

    LexerFlags _flags{LexerFlag::None};
    String     _input;
    Scanners   _scanners;
    size_type  _line{FIRST_LINE};
    size_type  _column{FIRST_COLUMN};
    size_type  _pos{0};
  };

} // namespace cs
