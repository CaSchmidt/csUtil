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

#include <cs/Text/StringUtilImpl.h>

#include <cs/Core/CharUtil.h>

namespace cs {

  namespace impl_string {

    ////// String contains character... //////////////////////////////////////

    template<typename CharT>
    inline bool contains(const std::basic_string_view<CharT>& text,
                         const CharT pattern)
    {
      return std::find(text.begin(), text.end(), pattern) != text.end();
    }

    ////// String contains predicate... //////////////////////////////////////

    template<typename CharT, typename UnaryPred>
    inline bool contains(const std::basic_string_view<CharT>& text,
                         UnaryPred pred)
    {
      return std::find_if(text.begin(), text.end(), pred) != text.end();
    }

    ////// String contains pattern... ////////////////////////////////////////

    template<typename CharT>
    inline bool contains(const std::basic_string_view<CharT>& text,
                         const std::basic_string_view<CharT>& pattern,
                         const bool ignoreCase)
    {
      constexpr auto NPOS = std::basic_string_view<CharT>::npos;

      if( !isTextPattern<CharT>(text, pattern) ) {
        return false;
      }

      return ignoreCase
          ? std::search(text.begin(), text.end(), pattern.begin(), pattern.end(), lambda_eqI<CharT>()) != text.end()
          : text.find(pattern) != NPOS;
    }

    ////// String ends with pattern... ///////////////////////////////////////

    template<typename CharT>
    inline bool endsWith(const std::basic_string_view<CharT>& text,
                         const std::basic_string_view<CharT>& pattern,
                         const bool ignoreCase)
    {
      if( !isTextPattern<CharT>(text, pattern) ) {
        return false;
      }

      return ignoreCase
          ? std::equal(pattern.rbegin(), pattern.rend(), text.rbegin(), lambda_eqI<CharT>())
          : std::equal(pattern.rbegin(), pattern.rend(), text.rbegin());
    }

    ////// String are equal... ///////////////////////////////////////////////

    template<typename CharT>
    inline bool equals(const std::basic_string_view<CharT>& a,
                       const std::basic_string_view<CharT>& b,
                       const bool ignoreCase)
    {
      if( a.empty()  ||  a.size() != b.size() ) {
        return false;
      }

      return ignoreCase
          ? std::equal(a.begin(), a.end(), b.begin(), lambda_eqI<CharT>())
          : std::equal(a.begin(), a.end(), b.begin());
    }

    ////// String starts with pattern... /////////////////////////////////////

    template<typename CharT>
    inline bool startsWith(const std::basic_string_view<CharT>& text,
                           const std::basic_string_view<CharT>& pattern,
                           const bool ignoreCase)
    {
      if( !isTextPattern<CharT>(text, pattern) ) {
        return false;
      }

      return ignoreCase
          ? std::equal(pattern.begin(), pattern.end(), text.begin(), lambda_eqI<CharT>())
          : std::equal(pattern.begin(), pattern.end(), text.begin());
    }

  } // namespace impl_string

} // namespace cs
