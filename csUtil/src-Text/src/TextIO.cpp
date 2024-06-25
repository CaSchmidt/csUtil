/****************************************************************************
** Copyright (c) 2018, Carsten Schmidt. All rights reserved.
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

#include "cs/Text/TextIO.h"

#include "cs/Core/Container.h"
#include "cs/Text/StringUtil.h"
#include "cs/IO/File.h"

namespace cs {

  ////// Private /////////////////////////////////////////////////////////////

  namespace impl_textio {

    template<typename CharT>
    inline std::basic_string<CharT> readInput(std::basic_istream<CharT>& stream,
                                              const std::size_t count,
                                              const CharT delim)
    {
      using String = std::basic_string<CharT>;
      using Traits = std::char_traits<CharT>;

      String result;
      if( !resize(result, count, static_cast<CharT>(' ')) ) {
        return String();
      }

      stream.get(result.data(), result.size(), delim);
      result.resize(Traits::length(result.data()));

      return result;
    }

  } // namespace impl_textio

  ////// Public //////////////////////////////////////////////////////////////

  CS_UTIL_EXPORT std::string readInput(std::istream& stream,
                                       const std::size_t count, const char delim)
  {
    return impl_textio::readInput<char>(stream, count, delim);
  }

  CS_UTIL_EXPORT std::list<std::string> readLines(const std::filesystem::path& path,
                                                  const LineFlags flags)
  {
    constexpr auto lambda_is_blank = [](const std::string& str) -> bool {
      return str.empty()  ||  isSpace(str);
    };

    const std::string text = readTextFile(path);
    if( text.empty() ) {
      return std::list<std::string>{};
    }

    const SplitFlags split_flags = flags.testAny(LineFlag::Trim)
        ? SplitFlag::Trim
        : SplitFlag::None;

    std::list<std::string> lines = split(text, '\n', split_flags);
    if( flags.testAny(LineFlag::SkipBlank) ) {
      lines.remove_if(lambda_is_blank);
    }

    return lines;
  }

  CS_UTIL_EXPORT std::string readTextFile(const std::filesystem::path& path,
                                          bool *ok)
  {
    if( ok != nullptr ) {
      *ok = false;
    }

    File file;
    if( !file.open(path) ) {
      return std::string{};
    }
    const Buffer buffer = file.readAll();
    file.close();

    if( buffer.empty() ) {
      return std::string{};
    }

    const std::string text(reinterpret_cast<const char*>(buffer.data()), buffer.size());

    if( ok != nullptr ) {
      *ok = true;
    }

    return text;
  }

} // namespace cs
