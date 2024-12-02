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

#include <iterator>
#include <list>
#include <numeric>
#include <string>
#include <string_view>

#include <cs/Core/CharUtil.h>

namespace cs {

  namespace impl_string {

    template<typename CharT>
    constexpr auto join(const std::list<std::basic_string<CharT>>& list,
                        const CharT pattern)
    -> std::basic_string<CharT>
    {
      using      g = glyph<CharT>;
      using String = std::basic_string<CharT>;

      auto lambda_join = [=](String init, const String& item) -> String {
        if( pattern != g::NUL ) {
          init += pattern;
        }

        init += item;

        return init;
      };

      return !list.empty()
          ? std::accumulate(std::next(list.begin()), list.end(), list.front(), lambda_join)
          : String();
    }

    template<typename CharT>
    constexpr auto join(const std::list<std::basic_string<CharT>>& list,
                        const std::basic_string_view<CharT>& pattern)
    -> std::basic_string<CharT>
    {
      using String = std::basic_string<CharT>;

      auto lambda_join = [&](String init, const String& item) -> String {
        init += pattern;

        init += item;

        return init;
      };

      return !list.empty()  &&  !pattern.empty()
          ? std::accumulate(std::next(list.begin()), list.end(), list.front(), lambda_join)
          : String();
    }

  } // namespace impl_string

} // namespace cs
