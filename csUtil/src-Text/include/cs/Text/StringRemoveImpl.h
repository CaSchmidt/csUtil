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

#include <algorithm>
#include <iterator>

#include <cs/Text/StringUtilImpl.h>

namespace cs {

  namespace impl_string {

    ////// Remove pattern from string... /////////////////////////////////////

    template<typename CharT>
    inline void removeAll(std::basic_string<CharT>& text,
                          const std::basic_string_view<CharT>& pattern)
    {
      using size_type = typename std::basic_string<CharT>::size_type;

      constexpr auto NPOS = std::basic_string<CharT>::npos;

      if( !isTextPattern<CharT>(text, pattern) ) {
        return;
      }

      for(size_type hit = 0; (hit = text.find(pattern, hit)) != NPOS; ) {
        text.erase(hit, pattern.size());
      }
    }

    ////// Remove characters matching predicate from string... ///////////////

    template<typename CharT, typename UnaryPred>
    inline void removeAll(std::basic_string<CharT>& text,
                          UnaryPred pred)
    {
      using CIter = typename std::basic_string<CharT>::const_iterator;

      for(CIter hit = text.cbegin(); (hit = std::find_if(hit, text.cend(), pred)) != text.cend(); ) {
        text.erase(hit);
      }
    }

    ////// Remove trailing zeros from Fixed-Notation Floating-Point string ///

    template<typename CharT>
    inline void removeTrailingZeros(std::basic_string<CharT>& text,
                                    const bool removeDot)
    {
      constexpr auto lambda_is_exp = [](const CharT ch) -> bool {
        constexpr CharT _e = 'e';
        constexpr CharT _E = 'E';

        return ch == _e  ||  ch == _E;
      };

      constexpr auto lambda_is_zero = [](const CharT ch) -> bool {
        constexpr CharT _0 = '0';

        return ch == _0;
      };

      using    CIter = typename std::basic_string<CharT>::const_iterator;
      using CRevIter = typename std::basic_string<CharT>::const_reverse_iterator;

      constexpr CharT DOT = '.';

      // (1) Find '.' ////////////////////////////////////////////////////////

      const CIter iterDot = std::find(text.begin(), text.end(), DOT);
      if( iterDot == text.end() ) {
        return;
      }

      // (2) Do not touch exponent notation! /////////////////////////////////

      if( std::find_if(std::next(iterDot), text.cend(), lambda_is_exp) != text.end() ) {
        return;
      }

      // (3) Remove trailing zeros ///////////////////////////////////////////

      const CIter iterZero = std::find_if_not(text.crbegin(), CRevIter(iterDot),
                                              lambda_is_zero).base();
      if( iterZero != text.end() ) {
        text.erase(iterZero, text.end());
      }

      // (4) (Optionally) Remove trailing '.' ////////////////////////////////

      if( removeDot  &&  text.back() == DOT ) {
        text.pop_back();
      }
    }

  } // namespace impl_string

} // namespace cs
