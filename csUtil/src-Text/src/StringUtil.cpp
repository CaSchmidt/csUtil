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

#include "cs/Text/StringUtil.h"

#include "cs/Text/StringCaseImpl.h"
#include "cs/Text/StringCastImpl.h"
#include "cs/Text/StringClassifyImpl.h"
#include "cs/Text/StringSplitImpl.h"

namespace cs {

  // isClass() ///////////////////////////////////////////////////////////////

  bool CS_UTIL_EXPORT isHexString(const std::string_view& text)
  {
    return impl_string::isHexString<char>(text);
  }

  bool CS_UTIL_EXPORT isIdent(const std::string_view& text)
  {
    return impl_string::isIdent<char>(text);
  }

  bool CS_UTIL_EXPORT isSpace(const std::string_view& text)
  {
    return impl_string::isSpace<char>(text);
  }

  // lower() /////////////////////////////////////////////////////////////////

  void CS_UTIL_EXPORT lower(std::string& text)
  {
    impl_string::lower<char>(text);
  }

  std::string CS_UTIL_EXPORT toLower(std::string text)
  {
    lower(text);
    return text;
  }

  // narrow() ////////////////////////////////////////////////////////////////

  std::string CS_UTIL_EXPORT toString(const std::u16string_view& text)
  {
    return impl_string::toNarrow<char,char16_t>(text);
  }

  std::string CS_UTIL_EXPORT toString(const std::wstring_view& text)
  {
    return impl_string::toNarrow<char,wchar_t>(text);
  }

  // shrink() ////////////////////////////////////////////////////////////////

  void CS_UTIL_EXPORT shrink(std::string& str, const bool reclaim)
  {
    impl_string::shrink<char>(str, reclaim);
  }

  // simplify() //////////////////////////////////////////////////////////////

  void CS_UTIL_EXPORT simplify(std::string& text)
  {
    impl_string::simplify<char>(text);
  }

  std::string CS_UTIL_EXPORT toSimplified(std::string text)
  {
    simplify(text);
    return text;
  }

  // split() /////////////////////////////////////////////////////////////////

  std::list<std::string> CS_UTIL_EXPORT split(const std::string_view& text,
                                              const char pattern,
                                              const SplitFlags flags)
  {
    return impl_string::split<char>(text, std::string_view(std::addressof(pattern), 1),
                                    flags.testAny(SplitFlag::SkipEmpty),
                                    flags.testAny(SplitFlag::Trim));
  }

  std::list<std::string> CS_UTIL_EXPORT split(const std::string_view& text,
                                              const std::string_view& pattern,
                                              const SplitFlags flags)
  {
    return impl_string::split<char>(text, pattern,
                                    flags.testAny(SplitFlag::SkipEmpty),
                                    flags.testAny(SplitFlag::Trim));
  }

  // trim() //////////////////////////////////////////////////////////////////

  void CS_UTIL_EXPORT trim(std::string& text)
  {
    impl_string::trim<char>(text);
  }

  std::string CS_UTIL_EXPORT toTrimmed(std::string text)
  {
    trim(text);
    return text;
  }

  // upper() /////////////////////////////////////////////////////////////////

  void CS_UTIL_EXPORT upper(std::string& text)
  {
    impl_string::upper<char>(text);
  }

  std::string CS_UTIL_EXPORT toUpper(std::string text)
  {
    upper(text);
    return text;
  }

  // widen() /////////////////////////////////////////////////////////////////

  std::u16string CS_UTIL_EXPORT toUtf16String(const std::string_view& text)
  {
    return impl_string::toWide<char16_t,char>(text);
  }

  std::wstring CS_UTIL_EXPORT toWString(const std::string_view& text)
  {
    return impl_string::toWide<wchar_t,char>(text);
  }

} // namespace cs
