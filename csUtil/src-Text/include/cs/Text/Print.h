/****************************************************************************
** Copyright (c) 2022, Carsten Schmidt. All rights reserved.
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

#include <sstream>
#include <utility>

namespace cs {

  namespace impl_print {

    template<typename CharT>
    void print(std::basic_ostream<CharT>& stream, const std::basic_string_view<CharT>& view)
    {
      stream << view;
    }

    template<typename CharT, typename T, typename ...Args>
    void print(std::basic_ostream<CharT>& stream, const std::basic_string_view<CharT>& view,
               T value, Args&&... args)
    {
      constexpr auto NPOS = std::basic_string_view<CharT>::npos;
      constexpr auto  ONE = typename std::basic_string_view<CharT>::size_type{1};
      constexpr auto  PCT = static_cast<CharT>('%');

      const auto hitpos = view.find(PCT);
      if( hitpos == NPOS ) {
        print<CharT>(stream, view);
        return;
      }

      print<CharT>(stream, view.substr(0, hitpos));
      stream << value;
      print<CharT,Args...>(stream, view.substr(hitpos + ONE), std::forward<Args>(args)...);
    }

    template<typename CharT, typename ...Args>
    void println(std::basic_ostream<CharT>& stream, const std::basic_string_view<CharT>& fmt,
                 Args&&... args)
    {
      print<CharT,Args...>(stream, fmt, std::forward<Args>(args)...);
      stream << std::endl;
    }

    template<typename CharT, typename ...Args>
    std::basic_string<CharT> sprint(const std::basic_string_view<CharT>& fmt,
                                    Args&& ...args)
    {
      std::basic_ostringstream<CharT> stream;
      print<CharT,Args...>(stream, fmt, std::forward<Args>(args)...);
      return stream.str();
    }

  } // namespace impl_print

} // namespace cs
