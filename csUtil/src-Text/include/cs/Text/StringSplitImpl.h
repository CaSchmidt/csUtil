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

#include <list>

#include <cs/Text/StringStyleImpl.h>
#include <cs/Text/StringUtilImpl.h>

namespace cs {

  namespace impl_string {

    template<typename CharT>
    inline void extract(std::list<std::basic_string<CharT>>& result,
                        const std::basic_string_view<CharT>& text,
                        const std::size_t from, const std::size_t hit,
                        const bool do_skip_empty, const bool do_trim)
    {
      std::basic_string<CharT> part = from < hit
          ? std::basic_string<CharT>(text.substr(from, hit - from))
          : std::basic_string<CharT>();

      if( do_trim ) {
        trim(part);
      }

      if( do_skip_empty  &&  part.empty() ) {
        return;
      }

      result.push_back(std::move(part));
    }

    template<typename CharT>
    inline std::list<std::basic_string<CharT>> split(const std::basic_string_view<CharT>& text,
                                                     const std::basic_string_view<CharT>& pattern,
                                                     const bool do_skip_empty, const bool do_trim)
    {
      constexpr auto NPOS = std::basic_string_view<CharT>::npos;

      std::list<std::basic_string<CharT>> result;

      if( !isTextPattern<CharT>(text, pattern) ) {
        return result;
      }

      std::size_t from = 0;
      for(std::size_t hit = 0; (hit = text.find(pattern, from)) != NPOS;
          from = hit + pattern.size()) {
        extract<CharT>(result, text, from, hit, do_skip_empty, do_trim);
      }
      extract<CharT>(result, text, from, text.size(), do_skip_empty, do_trim);

      return result;
    }

  } // namespace impl_string

} // namespace cs
