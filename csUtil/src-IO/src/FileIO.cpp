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

#include "cs/IO/FileIO.h"

#include "cs/Text/StringUtil.h"
#include "cs/IO/File.h"

namespace cs {

  ////// Public ////////////////////////////////////////////////////////////////

  CS_UTIL_EXPORT std::list<std::string> readLines(const std::filesystem::path& path,
                                                  const bool skipBlank,
                                                  const bool doTrim)
  {
    constexpr auto lambda_is_blank = [](const String<char>& str) -> bool {
      return str.empty()  ||  isSpace(str);
    };

    const std::string text = readTextFile(path);
    if( text.empty() ) {
      return StringList<char>();
    }

    StringList<char> lines = split(text, '\n', false, doTrim);
    if( skipBlank ) {
      lines.remove_if(lambda_is_blank);
    }

    return lines;
  }

  CS_UTIL_EXPORT std::string readTextFile(const std::filesystem::path& path, bool *ok)
  {
    using Buffer = std::vector<uint8_t>;

    if( ok != nullptr ) {
      *ok = false;
    }

    File file;
    if( !file.open(path) ) {
      return String<char>();
    }
    const Buffer buffer = file.readAll();
    file.close();

    if( buffer.empty() ) {
      return String<char>();
    }

    const String<char> text(reinterpret_cast<const char*>(buffer.data()), buffer.size());

    if( ok != nullptr ) {
      *ok = true;
    }

    return text;
  }

} // namespace cs
