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
#include <iterator>
#include <string_view>
#include <tuple>

#include <cs/Core/CRTPbase.h>

namespace cs {

  namespace impl_mixin {
    using namespace std::string_view_literals;

    template<typename CharT>
    struct FormatTo {
      // SFINAE
    };

    template<>
    struct FormatTo<char> {
      static constexpr std::string_view FMT = "{}{}"sv;

      static constexpr std::string_view SEP = ", "sv;
    };

    template<>
    struct FormatTo<wchar_t> {
      static constexpr std::wstring_view FMT = L"{}{}"sv;

      static constexpr std::wstring_view SEP = L", "sv;
    };

  } // namespace impl_mixin

  /*
   * NOTE: 'DerivedT' must implement:
   *
   * auto DerivedT::elements() const
   * {
   *   return std::forward_as_tuple(...);
   * }
   */

  template<typename DerivedT>
  struct ToStringMixIn : CRTPbase<ToStringMixIn<DerivedT>> {

    template<typename CharT>
    std::basic_string<CharT> toString(const std::basic_string_view<CharT>& sep) const
    {
      using FormatTo   = impl_mixin::FormatTo<CharT>;
      using String     = std::basic_string<CharT>;
      using StringView = std::basic_string_view<CharT>;

      String buffer;

      const auto& elems = this->as_derived().elements();
      std::apply([&buffer,sep](const auto& ...args) -> void {
        std::size_t i{0};
        (std::format_to(std::back_inserter(buffer), FormatTo::FMT,
                        args,
                        ++i != sizeof...(args) ? sep : StringView()), ...);
      }, elems);

      return buffer;
    }

    inline std::string toString() const
    {
      return toString<char>(impl_mixin::FormatTo<char>::SEP);
    }

    inline std::wstring toWString() const
    {
      return toString<wchar_t>(impl_mixin::FormatTo<wchar_t>::SEP);
    }
  }; // struct ToStringMixIn<>

  template<typename DerivedT, typename CharT>
  struct ToString_formatter : public std::formatter<std::basic_string_view<CharT>,CharT> {
    using Formatter = std::formatter<std::basic_string_view<CharT>,CharT>;

    template<class FormatContext>
    inline typename FormatContext::iterator format(const ToStringMixIn<DerivedT>& mixin,
                                                   FormatContext& ctx) const
    {
      using FormatTo = impl_mixin::FormatTo<CharT>;

      return Formatter::format(mixin.template toString<CharT>(FormatTo::SEP), ctx);
    }
  };

  template<typename DerivedT>
  inline std::ostream& operator<<(std::ostream& stream,
                                  const ToStringMixIn<DerivedT>& derived)
  {
    stream << derived.toString();
    return stream;
  }

  template<typename DerivedT>
  inline std::wostream& operator<<(std::wostream& stream,
                                   const ToStringMixIn<DerivedT>& derived)
  {
    stream << derived.toWString();
    return stream;
  }

} // namespace cs
