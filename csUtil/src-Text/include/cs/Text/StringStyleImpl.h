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
#include <string>

#include <cs/Core/CharUtil.h>

namespace cs {

  namespace impl_string {

    ////// Remove whitespace from begin & end of string... ///////////////////

    template<typename CharT>
    inline void trim(std::basic_string<CharT>& text)
    {
      using CIter = typename std::basic_string<CharT>::const_iterator;

      // (0) Sanitize input //////////////////////////////////////////////////

      if( std::all_of(text.begin(), text.end(), ::cs::lambda_is_space<CharT>()) ) {
        text.clear();
        return;
      }

      // (1) Trim left ///////////////////////////////////////////////////////

      const CIter lbegin = std::find_if_not(text.begin(), text.end(),
                                            ::cs::lambda_is_space<CharT>());
      if( lbegin != text.end() ) {
        text.erase(text.begin(), lbegin);
      }

      // (2) Trim right //////////////////////////////////////////////////////

      const CIter rend = std::find_if_not(text.crbegin(), text.crend(),
                                          ::cs::lambda_is_space<CharT>()).base();
      if( text.begin() != rend ) {
        text.erase(rend, text.end());
      }
    }

    ////// Replace consecutive whitespace with single space... ///////////////

    template<typename CharT>
    inline void simplify(std::basic_string<CharT>& text)
    {
      constexpr auto lambda_is_unique = [](const CharT lhs, const CharT rhs) -> bool {
        return ::cs::isSpace<CharT>(lhs)  &&  ::cs::isSpace<CharT>(rhs);
      };

      // (1) Remove consecutive whitespace ///////////////////////////////////

      const auto rend = std::unique(text.begin(), text.end(), lambda_is_unique);
      if( rend != text.end() ) {
        text.erase(rend, text.end());
      }

      // (2) Replace any whitespace with space ///////////////////////////////

      std::replace_if(text.begin(), text.end(),
                      ::cs::lambda_is_space<CharT>(), glyph<CharT>::space);

      // (3) Trim left ///////////////////////////////////////////////////////

      if( !text.empty()  &&  ::cs::isSpace<CharT>(text.front()) ) {
        text.erase(0, 1);
      }

      // (4) Trim right //////////////////////////////////////////////////////

      if( !text.empty()  &&  ::cs::isSpace<CharT>(text.back()) ) {
        text.pop_back();
      }
    }

  } // namespace impl_string

} // namespace cs
