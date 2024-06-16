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

#include <cs/Lexer/Context.h>
#include <cs/Lexer/AbstractParser.h>

#include <cs/Text/PrintFormat.h> // TODO
#include <cs/Text/PrintUtil.h>

namespace Calculate {

  enum CalculateTokens : cs::tokenid_t {
    TOK_Identifier = cs::Token::make_userid(1),
    TOK_Integral
  };

  class CalculateTokenNames : public cs::TokenNamesBase {
  private:
    struct ctor_tag {
      ctor_tag() noexcept = default;
    };

  public:
    CalculateTokenNames(const ctor_tag& = ctor_tag()) noexcept;

    ~CalculateTokenNames() noexcept;

    const char *name(const cs::tokenid_t id) const;

    static cs::TokenNamesPtr make();
  };

  template<typename T>
  requires std::is_unsigned_v<T>
  class Parser : public cs::AbstractParser<char> {
  public:
    using value_type = T;

    Parser() noexcept = default;
    ~Parser() noexcept = default;

    static std::string fixInput(const std::string& text)
    {
      std::string input;

      const auto lines = cs::split(text, '\n', cs::SplitFlag::SkipEmpty);
      for(std::string line : lines) {
        cs::simplify(line);
        line += '\n';

        input += line;
      }

      return input;
    }

  protected:
    bool initialize()
    {
      using ctx = cs::LexerContext<char>;

      _lexer.addScanner(ctx::CharLiteralScanner::make("+-.*/%()"));
      _lexer.addScanner(ctx::CIdentifierScanner::make(TOK_Identifier));
      _lexer.addScanner(ctx::CIntegralScanner<value_type>::make(TOK_Integral, true));

      _lexer.setFlags(cs::LexerFlag::ScanLF);

      _names = CalculateTokenNames::make();

      return true;
    }

    void start()
    {
      { // TODO
        const value_type result = parseExpr();

        cs::println("result = % (0x%)", result, cs::hexf(result));

        check('\n');
      } // TODO

      check(cs::TOK_EndOfInput);
    }

    value_type parsePrimary()
    {
      if(        isLookAhead(TOK_Identifier) ) {
        scan();
        return 0;

      } else if( isLookAhead(TOK_Integral) ) {
        scan();
        return currentValue<value_type>();

      } else if( isLookAhead('(') ) {
        scan();
        const value_type result = parseExpr();
        check(')');
        return result;

      }

      throwUnexpectedToken(_lookAheadToken);

      return 0;
    }

    value_type parseUnary()
    {
      if(        isLookAhead('+') ) {
        scan();
        return parsePrimary();
      } else if( isLookAhead('-') ) {
        scan();
        return minus(parsePrimary());
      } else {
        return parsePrimary();
      }

      return 0; // fall-through; can never be reached!
    }

    value_type parseExpr()
    {
      return parseUnary();
    }

  private:
    constexpr value_type minus(const value_type x)
    {
      constexpr value_type ONE = 1;

      return ~x + ONE;
    }
  };

} // namespace Calculate
