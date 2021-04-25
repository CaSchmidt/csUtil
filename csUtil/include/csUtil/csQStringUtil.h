/****************************************************************************
** Copyright (c) 2021, Carsten Schmidt. All rights reserved.
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

#ifndef CSQSTRINGUTIL_H
#define CSQSTRINGUTIL_H

#include <string>

#include <QtCore/QString>

#include <csUtil/csTypeTraits.h>

namespace cs {

  inline QString toQString(const char8_t *s)
  {
    return s != nullptr
        ? QString::fromUtf8(cs::CSTR(s))
        : QString();
  }

  inline QString toQString(const std::u8string& s)
  {
    return !s.empty()
        ? QString::fromUtf8(cs::CSTR(s.data()), int(s.size()))
        : QString();
  }

  inline QString toQString(const char16_t *s)
  {
    return s != nullptr
        ? QString::fromUtf16(s)
        : QString();
  }

  inline QString toQString(const std::u16string& s)
  {
    return !s.empty()
        ? QString::fromUtf16(s.data(), int(s.size()))
        : QString();
  }

  inline std::u8string toUtf8String(const QString& s)
  {
    using size_type = std::u8string::size_type;
    const QByteArray utf8 = s.toUtf8();
    return !utf8.isEmpty()
        ? std::u8string(cs::UTF8(utf8.constData()), size_type(utf8.size()))
        : std::u8string();
  }

  inline std::u16string toUtf16String(const QString& s)
  {
    using size_type = std::u16string::size_type;
    return !s.isEmpty()
        ? std::u16string(reinterpret_cast<const char16_t*>(s.utf16()), size_type(s.size()))
        : std::u16string();
  }

} // namespace cs

#endif // CSQSTRINGUTIL_H
