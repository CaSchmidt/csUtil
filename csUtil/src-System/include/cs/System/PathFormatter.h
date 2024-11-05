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

#if 1

/*
 * NOTE:
 * Formatting a std::filesystem::path will be supported with C++26.
 * This implementation serves as an intermediate solution!
 *
 * cf. P2845R8, "Formatting of std::filesystem::path"
 *
 * cf. "Formatting User-Defined Types",
 * https://fmt.dev/11.0/api/#formatting-user-defined-types
 */

#include <filesystem>
#include <format>

namespace std {

  namespace impl_formatter {

    template<typename CharT>
    struct converter {
      // SFINAE
    };

    template<>
    struct converter<char> {
      inline static std::string to_string(const std::filesystem::path& path)
      {
        return path.string();
      }
    };

    template<>
    struct converter<wchar_t> {
      inline static std::wstring to_string(const std::filesystem::path& path)
      {
        return path.wstring();
      }
    };

  } // namespace impl_formatter

  template<typename CharT>
  struct formatter<std::filesystem::path,CharT>
      : public std::formatter<std::basic_string_view<CharT>,CharT> {
    using Converter = impl_formatter::converter<CharT>;
    using Formatter = formatter<std::basic_string_view<CharT>,CharT>;

    template<class FormatContext>
    inline typename FormatContext::iterator format(const filesystem::path& path,
                                                   FormatContext& ctx) const
    {
      return Formatter::format(Converter::to_string(path), ctx);
    }
  };

} // namespace std

#endif
