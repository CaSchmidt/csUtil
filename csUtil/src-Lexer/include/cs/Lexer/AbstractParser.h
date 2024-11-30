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

#include <cs/Lexer/Lexer.h>
#include <cs/Logging/AbstractLogger.h>

namespace cs {

  class CS_UTIL_EXPORT AbstractParser {
  public:
    using char_type = Lexer::char_type;
    using    String = Lexer::String;

    AbstractParser();
    virtual ~AbstractParser();

    bool parse(String input, const LoggerPtr& logger);

  protected:
    AbstractParser(const AbstractParser&) = delete;
    AbstractParser& operator=(const AbstractParser&) = delete;

    static_assert( is_tokenid_char_v<char_type> );

    // Parser ////////////////////////////////////////////////////////////////

    void check(const tokenid_t expected);

    template<typename T>
    void checkValue(const tokenid_t expected, const T& value)
    {
      if( isLookAhead(expected) ) {
        if( isLookAheadValue(expected, value) ) {
          scan();
        } else {
          throwUnexpectedTokenValue(_lookAheadToken);
        }
      } else {
        throwUnexpectedToken(expected, _lookAheadToken);
      }
    }

    template<typename T>
    inline T currentValue(const T& defValue = T()) const
    {
      using Value = ValueToken<T>;

      const Value *ptr = dynamic_cast<const Value*>(_currentToken.get());

      return ptr != nullptr
          ? ptr->value()
          : defValue;
    }

    inline bool isLookAhead(const tokenid_t id) const;

    template<typename T>
    inline bool isLookAheadValue(const tokenid_t id, const T& value) const
    {
      using Value = ValueToken<T>;

      const Value *ptr = dynamic_cast<const Value*>(_lookAheadToken.get());

      return ptr != nullptr
          ? ptr->id() == id  &&  ptr->value() == value
          : false;
    }

    void scan();

    // Exceptions ////////////////////////////////////////////////////////////

    void throwErrorMessage(const std::size_t line, const std::string& message) const;
    void throwUnexpectedToken(const TokenPtr& token) const;
    void throwUnexpectedToken(const tokenid_t expected, const TokenPtr& unexpected) const;
    void throwUnexpectedTokenValue(const TokenPtr& token) const;

    // Variables /////////////////////////////////////////////////////////////

    TokenPtr      _currentToken{nullptr};
    TokenPtr      _lookAheadToken{nullptr};
    Lexer         _lexer;
    TokenNamesPtr _names{nullptr};

    // User Implementation ///////////////////////////////////////////////////

    virtual bool initialize() = 0;
    virtual void start() = 0;
  };

} // namespace cs
