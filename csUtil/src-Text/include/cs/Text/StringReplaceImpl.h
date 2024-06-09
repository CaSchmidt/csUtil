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

namespace cs {

  namespace impl_string {

    ////// Replace character in string... ////////////////////////////////////

    template<typename CharT>
    void replaceAll(std::basic_string<CharT>& str,
                    const CharT before,
                    const CharT after)
    {
      std::replace(str.begin(), str.end(), before, after);
    }

    ////// Replace character matching predicate in string... /////////////////

    template<typename CharT, typename UnaryPred>
    void replaceAll(std::basic_string<CharT>& str,
                    UnaryPred pred,
                    const CharT after)
    {
      std::replace_if(str.begin(), str.end(), pred, after);
    }

    ////// Replace pattern in string... //////////////////////////////////////

    template<typename CharT>
    inline void replaceAll(std::basic_string<CharT>& str,
                           const std::basic_string_view<CharT>& before,
                           const std::basic_string_view<CharT>& after)
    {
      constexpr auto NPOS = std::basic_string<CharT>::npos;

      if( str.empty()  ||  str.size() < before.size()  ||  after.empty() ) {
        return;
      }

      for(std::size_t pos = 0;
          (pos = str.find(before, pos)) != NPOS;
          pos += after.size()) {
        str.replace(pos, before.size(), after);
      }
    }

  } // namespace impl_string

} // namespace cs
