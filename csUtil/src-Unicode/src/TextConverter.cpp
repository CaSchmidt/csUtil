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

#include <cstring>

#include <algorithm>
#include <set>

#include <unicode/ucnv.h>
#include <unicode/ustring.h>

#include "cs/Unicode/TextConverter.h"

namespace cs {

  ////// Private /////////////////////////////////////////////////////////////

  namespace impl_txtcnv {

    // UTF-16 -> 8bit ////////////////////////////////////////////////////////

    template<typename CharT>
    inline std::basic_string<CharT> fromUnicode(UConverter *cnv, const UChar *s, std::size_t len)
    {
      std::basic_string<CharT> result;

      if( len == cs::MAX_SIZE_T  &&  s != nullptr ) {
        len = static_cast<std::size_t>(u_strlen(s));
      }

      if( s == nullptr  ||  len < 1 ) {
        return result;
      }

      UErrorCode err = U_ZERO_ERROR;
      const int32_t requiredSize = ucnv_fromUChars(cnv, nullptr, 0, s, static_cast<int32_t>(len), &err);

      try {
        result.resize(static_cast<std::size_t>(requiredSize), 0);
      } catch(...) {
        result.clear();
        return result;
      }

      err = U_ZERO_ERROR;
      ucnv_fromUChars(cnv,
                      reinterpret_cast<char*>(result.data()), static_cast<int32_t>(result.size()),
                      s, static_cast<int32_t>(len), &err);

      if( U_FAILURE(err) ) {
        result.clear();
      }

      return result;
    }

    template<typename CharT>
    inline std::basic_string<CharT> fromUnicode(const char *name, const UChar *s, const std::size_t len)
    {
      std::basic_string<CharT> result;

      UErrorCode err = U_ZERO_ERROR;
      UConverter *cnv = ucnv_open(name, &err);
      if( cnv == nullptr  ||  U_FAILURE(err) ) {
        return result;
      }

      result = fromUnicode<CharT>(cnv, s, len);

      ucnv_close(cnv);

      return result;
    }

    // 8bit -> UTF-16 ////////////////////////////////////////////////////////

    inline std::u16string toUnicode(UConverter *cnv, const char *s, std::size_t len)
    {
      std::u16string result;

      if( len == cs::MAX_SIZE_T  &&  s != nullptr ) {
        len = static_cast<std::size_t>(std::strlen(s));
      }

      if( s == nullptr  ||  len < 1 ) {
        return result;
      }

      UErrorCode err = U_ZERO_ERROR;
      const int32_t requiredSize = ucnv_toUChars(cnv, nullptr, 0, s, static_cast<int32_t>(len), &err);

      try {
        result.resize(static_cast<std::size_t>(requiredSize), 0);
      } catch(...) {
        result.clear();
        return result;
      }

      err = U_ZERO_ERROR;
      ucnv_toUChars(cnv,
                    reinterpret_cast<UChar*>(result.data()), static_cast<int32_t>(result.size()),
                    s, static_cast<int32_t>(len), &err);

      if( U_FAILURE(err) ) {
        result.clear();
      }

      return result;
    }

    inline std::u16string toUnicode(const char *name, const char *s, const std::size_t len)
    {
      std::u16string result;

      UErrorCode err = U_ZERO_ERROR;
      UConverter *cnv = ucnv_open(name, &err);
      if( cnv == nullptr  ||  U_FAILURE(err) ) {
        return result;
      }

      result = toUnicode(cnv, s, len);

      ucnv_close(cnv);

      return result;
    }

  } // namespace impl_txtcnv

  ////// TextConverterData ///////////////////////////////////////////////////

  /***************************************************************************
   * NOTE:
   * Although TextConverterData's destructor will be called whenever the enclosing
   * unique_ptr<> is destroyed, ICU specific memory leaks may be reported.
   * See the documentation of u_cleanup() for further information and the means to
   * mitigate the situation.
   ***************************************************************************/

  class TextConverterData {
  public:
    TextConverterData(const char *_name, UErrorCode *err) noexcept
      : cnv(nullptr)
      , name(_name)
    {
      *err = U_ZERO_ERROR;
      cnv = ucnv_open(_name, err);
    }

    ~TextConverterData() noexcept
    {
      if( cnv != nullptr ) {
        ucnv_close(cnv);
      }
    }

    UConverter *cnv;
    std::string name;
  };

  ////// public //////////////////////////////////////////////////////////////

  TextConverter::TextConverter(TextConverterData *ptr) noexcept
    : d(ptr)
  {
  }

  TextConverter::~TextConverter() noexcept = default;
  TextConverter::TextConverter(TextConverter&&) noexcept = default;
  TextConverter& TextConverter::operator=(TextConverter&&) noexcept = default;

  bool TextConverter::isNull() const
  {
    return !d;
  }

  void TextConverter::clear()
  {
    d.reset(nullptr);
  }

  const char *TextConverter::name() const
  {
    if( isNull() ) {
      return nullptr;
    }
    return d->name.data();
  }

  std::string TextConverter::fromUnicode(const char16_t *s, const std::size_t len) const
  {
    return impl_txtcnv::fromUnicode<char>(d->cnv, s, len);
  }

  std::u16string TextConverter::toUnicode(const char *s, const std::size_t len) const
  {
    return impl_txtcnv::toUnicode(d->cnv, s, len);
  }

  ////// public static ///////////////////////////////////////////////////////

  TextConverter TextConverter::create(const char *name)
  {
    TextConverter result;

    UErrorCode err;

    try {
      result.d = std::make_unique<TextConverterData>(name, &err);
    } catch(...) {
      result.clear();
      return result;
    }

    if( result.d->cnv == nullptr  ||  U_FAILURE(err) ) {
      result.clear();
      return result;
    }

    return result;
  }

  TextConverter TextConverter::createAscii()
  {
    return create("ASCII");
  }

  TextConverter TextConverter::createLatin1()
  {
    return create("ISO-8859-1");
  }

  TextConverter TextConverter::createLatin9()
  {
    return create("ISO-8859-15");
  }

  TextConverter TextConverter::createUtf8()
  {
    return create("UTF-8");
  }

  TextConverter TextConverter::createWindows1252()
  {
    return create("WINDOWS-1252");
  }

  std::list<std::string> TextConverter::listAvailable()
  {
    std::set<std::string> names;

    const int32_t numAvailable = ucnv_countAvailable();
    for(int32_t i = 0; i < numAvailable; i++) {
      UErrorCode err;

      const char *name = ucnv_getAvailableName(i);
      if( name == nullptr  ||  std::strlen(name) < 1 ) {
        continue;
      }

      names.insert(std::string(name));

      err = U_ZERO_ERROR;
      const uint16_t numAliases = ucnv_countAliases(name, &err);
      if( U_FAILURE(err) ) {
        continue;
      }

      for(uint16_t i = 0; i < numAliases; i++) {
        err = U_ZERO_ERROR;
        const char *alias = ucnv_getAlias(name, i, &err);
        if( U_FAILURE(err)  ||  alias == nullptr  ||  std::strlen(alias) < 1 ) {
          continue;
        }

        names.insert(std::string(alias));
      } // For each alias
    } // For each available name

    return std::list<std::string>(names.cbegin(), names.cend());
  }

  ////// Public //////////////////////////////////////////////////////////////

  CS_UTIL_EXPORT std::u16string asciiToUnicode(const char *s, const std::size_t len)
  {
    return impl_txtcnv::toUnicode("ASCII", s, len);
  }

  CS_UTIL_EXPORT std::u16string utf8ToUnicode(const char8_t *s, const std::size_t len)
  {
    return impl_txtcnv::toUnicode("UTF-8", cs::CSTR(s), len);
  }

  CS_UTIL_EXPORT std::string unicodeToAscii(const char16_t *s, const std::size_t len)
  {
    return impl_txtcnv::fromUnicode<char>("ASCII", s, len);
  }

  CS_UTIL_EXPORT std::u8string unicodeToUtf8(const char16_t *s, const std::size_t len)
  {
    return impl_txtcnv::fromUnicode<char8_t>("UTF-8", s, len);
  }

} // namespace cs
