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
#include <string_view>

#include <cs/Core/CharUtil.h>

namespace cs {

  namespace impl_string {

    ////// Prefix indicates numerical base... ////////////////////////////////

    template<typename CharT>
    inline bool isHexBase(const std::basic_string_view<CharT>& text)
    {
      using size_type = typename std::basic_string_view<CharT>::size_type;

      constexpr size_type TWO = 2;

      return text.size() >= TWO          &&
          text[0] == glyph<CharT>::zero  &&
          (text[1] == glyph<CharT>::x  ||  text[1] == glyph<CharT>::X);
    }

    template<typename CharT>
    inline bool isOctBase(const std::basic_string_view<CharT>& text)
    {
      return !text.empty()  &&  text[0] == glyph<CharT>::zero;
    }

    ////// String is hexadecimal string... ///////////////////////////////////

    template<typename CharT>
    inline bool isHexString(const std::basic_string_view<CharT>& text)
    {
      return !text.empty()  &&
          std::all_of(text.begin(), text.end(), lambda_is_hex<CharT>());
    }

    ////// String is C-style identifier... ///////////////////////////////////

    template<typename CharT>
    inline bool isIdent(const std::basic_string_view<CharT>& text)
    {
      return !text.empty()            &&
          isIdentFirst(text.front())  &&
          std::all_of(++text.begin(), text.end(), lambda_is_ident<CharT>());
    }

    ////// String contains only whitespace... ////////////////////////////////

    template<typename CharT>
    inline bool isSpace(const std::basic_string_view<CharT>& text)
    {
      return !text.empty()  &&
          std::all_of(text.begin(), text.end(), lambda_is_space<CharT>());
    }

  } // namespace impl_string

} // namespace cs
