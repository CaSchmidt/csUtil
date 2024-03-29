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

#pragma once

#include <list>
#include <memory>
#include <string>

#include <cs/Core/csutil_config.h>

#include <cs/Core/TypeTraits.h>

namespace cs {

  class TextConverterData;

  class CS_UTIL_EXPORT TextConverter {
  public:
    TextConverter(TextConverterData *ptr = nullptr) noexcept;
    ~TextConverter() noexcept;

    TextConverter(TextConverter&&) noexcept;
    TextConverter& operator=(TextConverter&&) noexcept;

    bool isNull() const;
    void clear();

    const char *name() const;

    std::string fromUnicode(const char16_t *s, const std::size_t len = MAX_SIZE_T) const;

    inline std::string fromUnicode(const std::u16string& s) const
    {
      return fromUnicode(s.data(), s.size());
    }

    std::u16string toUnicode(const char *s, const std::size_t len = MAX_SIZE_T) const;

    inline std::u16string toUnicode(const std::string& s) const
    {
      return toUnicode(s.data(), s.size());
    }

    static TextConverter create(const char *name);

    static TextConverter createAscii();
    static TextConverter createLatin1(); // ISO-8859-1
    static TextConverter createLatin9(); // ISO-8859-15
    static TextConverter createUtf8();
    static TextConverter createWindows1252();

    static std::list<std::string> listAvailable();

  private:
    TextConverter(const TextConverter&) = delete;
    TextConverter& operator=(const TextConverter&) = delete;

    std::unique_ptr<TextConverterData> d;
  };

  // 8bit -> UTF-16 ////////////////////////////////////////////////////////////

  CS_UTIL_EXPORT std::u16string asciiToUnicode(const char *s, const std::size_t len = MAX_SIZE_T);

  inline std::u16string asciiToUnicode(const std::string& s)
  {
    return asciiToUnicode(s.data(), s.size());
  }

  CS_UTIL_EXPORT std::u16string utf8ToUnicode(const char8_t *s, const std::size_t len = MAX_SIZE_T);

  inline std::u16string utf8ToUnicode(const std::u8string& s)
  {
    return utf8ToUnicode(s.data(), s.size());
  }

  // UTF-16 -> 8bit ////////////////////////////////////////////////////////////

  CS_UTIL_EXPORT std::string unicodeToAscii(const char16_t *s, const std::size_t len = MAX_SIZE_T);

  inline std::string unicodeToAscii(const std::u16string& s)
  {
    return unicodeToAscii(s.data(), s.size());
  }

  CS_UTIL_EXPORT std::u8string unicodeToUtf8(const char16_t *s, const std::size_t len = MAX_SIZE_T);

  inline std::u8string unicodeToUtf8(const std::u16string& s)
  {
    return unicodeToUtf8(s.data(), s.size());
  }

} // namespace cs
