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

#include <cs/Lexer/Scanner.h>

namespace cs {

  template<typename CharT>
  class Lexer {
  public:
    using String     = std::basic_string<CharT>;
    using value_type = typename String::value_type;
    using  size_type = typename String::size_type;
    using StringView = std::basic_string_view<value_type>;

    using Scanner  = ScannerPtr<value_type>;
    using Scanners = std::list<Scanner>;

    Lexer() noexcept
    {
      clear();
    }

    ~Lexer() noexcept
    {
    }

    bool addScanner(Scanner scanner)
    {
      if( !scanner ) {
        return false;
      }
      _scanners.push_back(std::move(scanner));
      return true;
    }

    void clear()
    {
      initialize(String());
      _scanners.clear();
    }

    void initialize(String input)
    {
      _input = std::move(input);
      _pos = _line = _column = 0;
    }

    TokenPtr nextToken()
    {
      // (1) Skip Space //////////////////////////////////////////////////////

      skip();

      // (2) End-of-Input reached? ///////////////////////////////////////////

      if( atEnd() ) {
        TokenPtr eoi = BaseToken::make(TOK_EndOfInput, 0);
        eoi->setLocation(_line, _column);
        return eoi;
      }

      // (3) Treat current character as unknown //////////////////////////////

      TokenPtr result = ValueToken<value_type>::make(TOK_Unknown, _input[_pos], 1);

      // (4) Scan input & take longest token /////////////////////////////////

      for(const Scanner& scanner : _scanners) {
        TokenPtr token = scanner->scan(view());
        if( !token ) {
          continue;
        }

        if( result->id() == TOK_Unknown ) {
          result = std::move(token); // No viable token yet, overwrite!
        } else if( token->size() > result->size() ) {
          result = std::move(token); // Take longest match!
        }
      }

      // (5) Set Location ////////////////////////////////////////////////////

      result->setLocation(_line, _column);

      // (6) Consume token ///////////////////////////////////////////////////

      consume(result->size());

      // Done! ///////////////////////////////////////////////////////////////

      return result;
    }

  private:
    static_assert( std::is_unsigned_v<size_type> );

    using g = glyph<value_type>;

    void advance(const value_type ch)
    {
      if( ch == g::LF ) {
        _line++;
        _column = 0;
      } else {
        _column++;
      }

      _pos++;
    }

    bool atEnd() const
    {
      return _pos >= _input.size();
    }

    void consume(const size_type _len)
    {
      const size_type len = std::min(_len, remaining());

      for(size_type i = 0; i < len; i++) {
        advance(_input[_pos]);
      }
    }

    size_type remaining() const
    {
      return _pos < _input.size()
          ? _input.size() - _pos
          : 0;
    }

    void skip()
    {
      while( _pos < _input.size() ) {
        const value_type ch = _input[_pos];

        if( isSpace(ch) ) {
          advance(ch);
        } else {
          break;
        }
      }
    }

    StringView view() const
    {
      const size_type numRemaining = remaining();
      return numRemaining > 0
          ? StringView(_input.data() + _pos, numRemaining)
          : StringView();
    }

    String    _input;
    Scanners  _scanners;
    size_type _line{0};
    size_type _column{0};
    size_type _pos{0};
  };

} // namespace cs
