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

#include <sstream>

#include <cs/Core/CharUtil.h>
#include <cs/Lexer/Lexer.h>
#include <cs/Lexer/ParseException.h>

namespace cs {

  template<typename CharT>
  requires is_tokenid_char_v<CharT>
  class BaseParser {
  public:
    using char_type = CharT;
    using    String = std::basic_string<char_type>;

    BaseParser()
    {
    }

    virtual ~BaseParser()
    {
    }

    bool parse(String input)
    {
      _currentToken.reset();
      _lookAheadToken.reset();
      _lexer.clear();
      _names = BaseTokenNames::make();

      if( !initialize() ) {
        fprintf(stderr, "ERROR: initialize()\n"); // TODO
        return false;
      }

      _lexer.initialize(std::move(input));
      scan();

      try {
        start();
      } catch( const parse_exception::base_exception& ex ) {
        fprintf(stderr, "ERROR:%d: %s\n",
                static_cast<int>(ex.line()),
                ex.what()); // TODO
        return false;
      }

      return true;
    }

  protected:
    // Parser ////////////////////////////////////////////////////////////////

    void check(const tokenid_t expected)
    {
      if( isLookAhead(expected) ) {
        scan();
      } else {
        throwUnexpectedToken(expected, _lookAheadToken);
      }
    }

    inline bool isLookAhead(const tokenid_t expected)
    {
      return _lookAheadToken  &&  _lookAheadToken->id() == expected;
    }

    void scan()
    {
      _currentToken   = std::move(_lookAheadToken);
      _lookAheadToken = _lexer.nextToken();
    }

    // Exceptions ////////////////////////////////////////////////////////////

    void throwUnexpectedToken(const TokenPtr& token)
    {
      using namespace parse_exception;

      throw unexpected_token(token, _names);
    }

    void throwUnexpectedToken(const tokenid_t expected, const TokenPtr& unexpected)
    {
      using namespace parse_exception;

      throw unexpected_token(expected, unexpected, _names);
    }

    void throwUnexpectedTokenValue(const TokenPtr& token)
    {
      using namespace parse_exception;

      throw unexpected_token_value(token, _names);
    }

    // Variables /////////////////////////////////////////////////////////////

    TokenPtr         _currentToken{nullptr};
    TokenPtr         _lookAheadToken{nullptr};
    Lexer<char_type> _lexer;
    TokenNamesPtr    _names{nullptr};

    // User Implementation ///////////////////////////////////////////////////

    virtual bool initialize() = 0;
    virtual void start() = 0;
  };

} // namespace cs
