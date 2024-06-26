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

#include <cs/Core/CharUtil.h>
#include <cs/Lexer/Scanner.h>
#include <cs/Lexer/ValueToken.h>

namespace cs {

  ////// C-String Scanner ////////////////////////////////////////////////////

  template<typename CharT>
  class CStringScanner : public IScanner<CharT> {
  private:
    struct ctor_tag {
      ctor_tag() noexcept = default;
    };

  public:
    using typename IScanner<CharT>::StringView;
    using typename IScanner<CharT>::size_type;
    using typename IScanner<CharT>::char_type;

    using String = std::basic_string<char_type>;

    CStringScanner(const tokenid_t id, const size_type reserve,
                   const ctor_tag& = ctor_tag()) noexcept
      : _id{id}
    {
      _reserve = reserve > 0
          ? reserve
          : RESERVE;
    }

    ~CStringScanner() noexcept
    {
    }

    TokenPtr scan(const StringView& input) const
    {
      // (1) Sanity Check ////////////////////////////////////////////////////

      if( input.size() < TWO  ||  input[0] != g::dblquote ) {
        return TokenPtr();
      }

      // (2) Reserve String //////////////////////////////////////////////////

      String str;
      try {
        str.reserve(_reserve);
      } catch( ... ) {
        return TokenPtr();
      }

      // (3) Scan Input & Build String ///////////////////////////////////////

      size_type pos = ONE; // start after the double quote!
      for(; pos < input.size(); pos++) {
        const char_type ch = input[pos];

        // (3.1) End-of-String ///////////////////////////////////////////////
        if( ch == g::dblquote ) {
          break;

          // (3.2) Escape Sequence ///////////////////////////////////////////
        } else if( ch == g::bckslash  &&  isEscape(lookAhead(pos, input)) ) {
          pos += ONE;
          str.push_back(escape(input[pos]));

          // (3.3) Character Exceptions //////////////////////////////////////
        } else if( isExcept(ch)  ) {
          return TokenPtr();

          // (3.4) Copy Literal Input Character //////////////////////////////
        } else {
          str.push_back(ch);

        }
      } // For input[pos]

      // (4) End-of-String Terminator Found? /////////////////////////////////

      if( pos >= input.size() ) {
        return TokenPtr();
      }

      // Done! ///////////////////////////////////////////////////////////////

      return ValueToken<String>::make(_id, str, pos + ONE);
    }

    static ScannerPtr<char_type> make(const tokenid_t id, const size_type reserve = 0)
    {
      return std::make_unique<CStringScanner<char_type>>(id, reserve);
    }

  private:
    CStringScanner() noexcept = delete;

    using g = glyph<char_type>;

    static constexpr size_type ONE = 1;
    static constexpr size_type TWO = 2;

    static constexpr size_type RESERVE = 1024;

    inline char_type escape(const char_type ch) const
    {
      if(        ch == g::sngquote ) {
        return ch;
      } else if( ch == g::dblquote ) {
        return ch;
      } else if( ch == g::qstnmark ) {
        return ch;
      } else if( ch == g::bckslash ) {
        return ch;
      } else if( ch == g::a ) {
        return g::BEL;
      } else if( ch == g::b ) {
        return g::BS;
      } else if( ch == g::f ) {
        return g::FF;
      } else if( ch == g::n ) {
        return g::LF;
      } else if( ch == g::r ) {
        return g::CR;
      } else if( ch == g::t ) {
        return g::HT;
      } else if( ch == g::v ) {
        return g::VT;
      }
      return g::NUL;
    }

    inline bool isEscape(const char_type ch) const
    {
      return
          ch == g::sngquote  || // \'
          ch == g::dblquote  || // \"
          ch == g::qstnmark  || // \?
          ch == g::bckslash  ||
          ch == g::a         || // \a
          ch == g::b         || // \b
          ch == g::f         || // \f
          ch == g::n         || // \n
          ch == g::r         || // \r
          ch == g::t         || // \t
          ch == g::v;           // \v
    }

    inline bool isExcept(const char_type ch) const
    {
      return
          ch == g::bckslash  ||
          ch == g::dblquote  ||
          ch == g::LF;
    }

    inline char_type lookAhead(const size_type pos, const StringView& input) const
    {
      return pos + ONE < input.size()
          ? input[pos + ONE]
          : g::NUL;
    }

    tokenid_t _id{0};
    size_type _reserve{0};
  };

} // namespace cs
