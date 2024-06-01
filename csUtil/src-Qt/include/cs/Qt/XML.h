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

#include <list>
#include <utility>

#include <QtXml/QDomElement>

#include <cs/Core/csutil_config.h>
#include <cs/Core/QStringUtil.h>

namespace cs {

  // Types ///////////////////////////////////////////////////////////////////

  using XmlAttribute = std::pair<QString,QString>;

  using XmlAttributes = std::list<XmlAttribute>;

  // Implementation - Create /////////////////////////////////////////////////

  QDomNode CS_UTIL_EXPORT xmlAppend(QDomNode& parent, const QString& name,
                                    const XmlAttributes& attributes);

  QDomNode CS_UTIL_EXPORT xmlAppend(QDomNode& parent, const QString& name,
                                    const XmlAttribute& attribute);

  QDomNode CS_UTIL_EXPORT xmlAppend(QDomNode& parent, const QString& name,
                                    const QString& value);

  QDomNode CS_UTIL_EXPORT xmlAppend(QDomNode& parent, const QString& name,
                                    const bool value);

  template<typename T>
  inline if_integral_t<T,QDomNode> xmlAppend(QDomNode& parent, const QString& name,
                                             const T value,
                                             const int base = 10)
  {
    return xmlAppend(parent, name, toQString<T>(value, base));
  }

  template<typename T>
  inline if_real_t<T,QDomNode> xmlAppend(QDomNode& parent, const QString& name,
                                         const T value,
                                         const char fmt = 'g',
                                         const int prec = 6)
  {
    return xmlAppend(parent, name, toQString<T>(value, fmt, prec));
  }

  // Implementation - Read ///////////////////////////////////////////////////

  namespace impl_xml {

    template<typename T>
    inline if_boolean_t<T> conv(const QString& text)
    {
      return text.trimmed().compare(QStringLiteral("true"), Qt::CaseInsensitive) == 0;
    }

    template<typename T>
    inline if_integral_t<T> conv(const QString& text)
    {
      return toValue<T>(text, nullptr, 0);
    }

    template<typename T>
    inline if_same_t<T,QString> conv(const QString& text)
    {
      return text;
    }

    template<typename T>
    inline T toValue(const QDomNode& node, const QString& name, const T& defValue)
    {
      const QDomElement elem = node.toElement();
      if( elem.isNull()  ||  elem.tagName() != name ) {
        return defValue;
      }

      return conv<T>(elem.text());
    }

  } // namespace impl_xml

  template<typename T>
  inline if_boolean_t<T> xmlToValue(const QDomNode& node, const QString& name)
  {
    return impl_xml::toValue<bool>(node, name, false);
  }

  template<typename T>
  inline if_integral_t<T> xmlToValue(const QDomNode& node, const QString& name)
  {
    return impl_xml::toValue<T>(node, name, 0);
  }

  template<typename T>
  inline if_same_t<T,QString> xmlToValue(const QDomNode& node, const QString& name)
  {
    return impl_xml::toValue<QString>(node, name, QString());
  }

} // namespace cs
