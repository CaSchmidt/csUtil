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

#include <format>
#include <string_view>

#include <N4/Color3f.h>
#include <N4/Matrix4f.h>
#include <N4/Normal3f.h>
#include <N4/Vertex4f.h>

namespace std {

  namespace impl_formatter {
    using namespace string_view_literals;

    template<typename CharT>
    struct konst {
      // SFINAE
    };

    template<>
    struct konst<char> {
      static constexpr std::string_view EOL = "\n"sv;

      static constexpr std::string_view SEP = "  "sv;
    };

    template<>
    struct konst<wchar_t> {
      static constexpr std::wstring_view EOL = L"\n"sv;

      static constexpr std::wstring_view SEP = L"  "sv;
    };

    template<typename Vec4f, typename CharT>
    struct vec4f_formatter : public std::formatter<typename Vec4f::value_type,CharT> {
      using     Konst = konst<CharT>;
      using Formatter = std::formatter<typename Vec4f::value_type,CharT>;

      template<class FormatContext>
      inline typename FormatContext::iterator format(const Vec4f& v,
                                                     FormatContext& ctx) const
      {
        auto&& out = ctx.out();

        Formatter::format(v(0), ctx);
        format_to(out, Konst::EOL);
        Formatter::format(v(1), ctx);
        format_to(out, Konst::EOL);
        Formatter::format(v(2), ctx);
        format_to(out, Konst::EOL);
        Formatter::format(v(3), ctx);
        format_to(out, Konst::EOL);

        return out;
      }
    };

  } // namespace impl_formatter

  template<typename CharT>
  struct formatter<n4::Color3f,CharT> : public impl_formatter::vec4f_formatter<n4::Color3f,CharT> {
  };

  template<typename CharT>
  struct formatter<n4::Normal3f,CharT> : public impl_formatter::vec4f_formatter<n4::Normal3f,CharT> {
  };

  template<typename CharT>
  struct formatter<n4::Vertex4f,CharT> : public impl_formatter::vec4f_formatter<n4::Vertex4f,CharT> {
  };

  template<typename CharT>
  struct formatter<n4::Matrix4f,CharT> : public std::formatter<n4::Matrix4f::value_type,CharT> {
    using     Konst = impl_formatter::konst<CharT>;
    using Formatter = std::formatter<n4::Matrix4f::value_type,CharT>;

    template<class FormatContext>
    inline typename FormatContext::iterator format(const n4::Matrix4f& M,
                                                   FormatContext& ctx) const
    {
      using size_type = n4::Matrix4f::size_type;

      auto&& out = ctx.out();

      for(size_type row = 0; row < M.rows(); row++) {
        for(size_type col = 0; col < M.columns(); col++) {
          format_to(out, Konst::SEP);
          Formatter::format(M(row, col), ctx);
        }

        format_to(out, Konst::EOL);
      } // For Each Row

      return out;
    }
  };

} // namespace std
