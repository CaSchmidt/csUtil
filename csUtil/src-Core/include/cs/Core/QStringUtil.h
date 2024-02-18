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

#pragma once

#include <filesystem>
#include <string>

#include <QtCore/QString>

#include <cs/Core/TypeTraits.h>

namespace cs {

  namespace impl_qt {

    using size_type = decltype(std::declval<QString>().size());

    constexpr auto MAX_QT_SIZE = maxab_v<size_type,std::size_t>;

    constexpr auto MAX_STD_SIZE = maxab_v<std::size_t,size_type>;

  } // namespace impl_qt

  inline QChar L1C(const char c)
  {
    return QChar::fromLatin1(c);
  }

  inline QString toQString(const char8_t *s)
  {
    return s != nullptr
        ? QString::fromUtf8(CSTR(s))
        : QString();
  }

  inline QString toQString(const std::u8string_view& s)
  {
    return !s.empty()  &&  s.size() <= impl_qt::MAX_STD_SIZE
        ? QString::fromUtf8(CSTR(s.data()), int(s.size()))
        : QString();
  }

  inline QString toQString(const std::u8string& s)
  {
    return toQString(static_cast<std::u8string_view>(s));
  }

  inline QString toQString(const char16_t *s)
  {
    return s != nullptr
        ? QString::fromUtf16(s)
        : QString();
  }

  inline QString toQString(const std::u16string_view& s)
  {
    return !s.empty()  &&  s.size() <= impl_qt::MAX_STD_SIZE
        ? QString::fromUtf16(s.data(), int(s.size()))
        : QString();
  }

  inline QString toQString(const std::u16string& s)
  {
    return toQString(static_cast<std::u16string_view>(s));
  }

  inline std::u8string toUtf8String(const QString& s)
  {
    using size_type = std::u8string::size_type;

    const QByteArray utf8 = s.toUtf8();
    return !utf8.isEmpty()  &&  utf8.size() <= impl_qt::MAX_QT_SIZE
        ? std::u8string(UTF8(utf8.constData()), size_type(utf8.size()))
        : std::u8string();
  }

  inline std::u16string toUtf16String(const QString& s)
  {
    return !s.isEmpty()
        ? s.toStdU16String()
        : std::u16string();
  }

  // Path Conversion /////////////////////////////////////////////////////////

  inline std::filesystem::path toPath(const QString& s,
                                      const std::filesystem::path::format fmt = std::filesystem::path::auto_format)
  {
    return !s.isEmpty()
        ? std::filesystem::path(s.toStdU16String(), fmt)
        : std::filesystem::path();
  }

  inline QString toQString(const std::filesystem::path& p)
  {
    return !p.empty()
        ? QString::fromStdU16String(p.generic_u16string())
        : QString();
  }

  // Value Conversion ////////////////////////////////////////////////////////

  template<typename T>
  inline if_integral_t<T,QString> toQString(const T value,
                                            const int base = 10)
  {
    return QString::number(value, base);
  }

  template<typename T>
  inline if_real_t<T,QString> toQString(const T value,
                                        const char fmt = 'g',
                                        const int prec = 6)
  {
    return QString::number(value, fmt, prec);
  }

  template<typename T>
  inline if_same_t<T,int> toValue(const QString& s,
                                  bool *ok = nullptr, const int base = 10)
  {
    return s.toInt(ok, base);
  }

  template<typename T>
  inline if_same_t<T,unsigned int> toValue(const QString& s,
                                           bool *ok = nullptr, const int base = 10)
  {
    return s.toUInt(ok, base);
  }

  template<typename T>
  inline if_same_t<T,long long> toValue(const QString& s,
                                        bool *ok = nullptr, const int base = 10)
  {
    return s.toLongLong(ok, base);
  }

  template<typename T>
  inline if_same_t<T,unsigned long long> toValue(const QString& s,
                                                 bool *ok = nullptr, const int base = 10)
  {
    return s.toULongLong(ok, base);
  }

  template<typename T>
  inline if_same_t<T,double> toValue(const QString& s,
                                     bool *ok = nullptr, const int = 0)
  {
    return s.toDouble(ok);
  }

  template<typename T>
  inline if_same_t<T,float> toValue(const QString& s,
                                    bool *ok = nullptr, const int = 0)
  {
    return s.toFloat(ok);
  }

} // namespace cs
