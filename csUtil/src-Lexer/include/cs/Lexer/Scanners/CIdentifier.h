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

  template<typename CharT>
  class CIdentifierScanner : public IScanner<CharT> {
  private:
    struct ctor_tag {
      ctor_tag() noexcept = default;
    };

  public:
    using typename IScanner<CharT>::StringView;
    using typename IScanner<CharT>::size_type;
    using typename IScanner<CharT>::value_type;

    using String = std::basic_string<value_type>;

    CIdentifierScanner(const tokenid_t id, const size_type reserve,
                       const ctor_tag& = ctor_tag()) noexcept
      : _id{id}
    {
      _reserve = reserve > 0
          ? reserve
          : RESERVE;
    }

    ~CIdentifierScanner() noexcept
    {
    }

    TokenPtr scan(const StringView& input) const
    {
      // (1) Sanity Check ////////////////////////////////////////////////////

      if( input.empty()  ||  !isIdentFirst(input[0]) ) {
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

      str.push_back(input[0]); // already checked; cf. above!

      size_type pos = ONE; // start at the 2nd character!
      for(; pos < input.size() ; pos++) {
        const value_type ch = input[pos];

        if( isIdent(ch) ) {
          str.push_back(ch);
        } else {
          break;
        }
      } // For input[pos]

      // Done! ///////////////////////////////////////////////////////////////

      // NOTE: 'pos' is at the first non-identifier character OR input.size()!
      return ValueToken<String>::make(_id, str, pos);
    }

    static ScannerPtr<value_type> make(const tokenid_t id, const size_type reserve = 1024)
    {
      return std::make_unique<CIdentifierScanner<value_type>>(id, reserve);
    }

  private:
    CIdentifierScanner() noexcept = delete;

    static constexpr size_type ONE = 1;

    static constexpr size_type RESERVE = 1024;

    tokenid_t _id{0};
    size_type _reserve{0};
  };

} // namespace cs
