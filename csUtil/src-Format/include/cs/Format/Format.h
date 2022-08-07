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

#ifndef CS_FORMAT_H
#define CS_FORMAT_H

#include <cstdint>

#include <memory>
#include <string>

#include <cs/Core/csutil_config.h>

namespace cs {
  namespace impl_format {
    template<typename CharT>
    class Formatter;
  }

  struct FormatChar {
    inline explicit FormatChar(const char& c)
      : value(c)
    {
    }

    inline explicit FormatChar(const signed char& c)
      : value(static_cast<char>(c))
    {
    }

    inline explicit FormatChar(const unsigned char& c)
      : value(static_cast<char>(c))
    {
    }

    const char value;
  };

  class CS_UTIL_EXPORT Format {
  public:
    Format(const std::string& s);
    Format(const char *s);
    ~Format();

    operator std::string() const;

    Format& arg(const FormatChar& c, const int width = 0, const char fill = ' ');

    Format& arg(const std::string& s, const int width = 0, const char fill = ' ');
    Format& arg(const char *s, const int width = 0, const char fill = ' ');

    Format& arg(const int8_t value, const int width = 0, const int base = 10, const char fill = ' ');
    Format& arg(const uint8_t value, const int width = 0, const int base = 10, const char fill = ' ');
    Format& arg(const int16_t value, const int width = 0, const int base = 10, const char fill = ' ');
    Format& arg(const uint16_t value, const int width = 0, const int base = 10, const char fill = ' ');
    Format& arg(const int32_t value, const int width = 0, const int base = 10, const char fill = ' ');
    Format& arg(const uint32_t value, const int width = 0, const int base = 10, const char fill = ' ');
    Format& arg(const int64_t value, const int width = 0, const int base = 10, const char fill = ' ');
    Format& arg(const uint64_t value, const int width = 0, const int base = 10, const char fill = ' ');

    Format& arg(const float value, const int width = 0, const char format = 'g', const int precision = 6, const char fill = ' ');
    Format& arg(const double value, const int width = 0, const char format = 'g', const int precision = 6, const char fill = ' ');

    Format& operator%(const FormatChar& c);

    Format& operator%(const std::string& s);
    Format& operator%(const char *s);

    Format& operator%(const int8_t value);
    Format& operator%(const uint8_t value);
    Format& operator%(const int16_t value);
    Format& operator%(const uint16_t value);
    Format& operator%(const int32_t value);
    Format& operator%(const uint32_t value);
    Format& operator%(const int64_t value);
    Format& operator%(const uint64_t value);

    Format& operator%(const float value);
    Format& operator%(const double value);

  private:
    using Formatter = impl_format::Formatter<char>;

    Format() = delete;
    Format(const Format&) = delete;
    Format& operator=(const Format&) = delete;
    Format(Format&&) = delete;
    Format& operator=(Format&&) = delete;

    std::unique_ptr<Formatter> impl;
  };

} // namespace cs

#endif // CS_FORMAT_H
