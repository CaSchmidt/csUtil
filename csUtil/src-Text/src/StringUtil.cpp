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

#include <memory>

#include "cs/Text/StringUtil.h"

#include "cs/Text/StringCaseImpl.h"
#include "cs/Text/StringCastImpl.h"
#include "cs/Text/StringClassifyImpl.h"
#include "cs/Text/StringSplitImpl.h"

namespace cs {

  // contains() //////////////////////////////////////////////////////////////

  bool CS_UTIL_EXPORT contains(const std::string_view& text,
                               const char pattern)
  {
    return impl_string::contains<char>(text, pattern);
  }

  bool CS_UTIL_EXPORT contains(const std::string_view& text,
                               const std::string_view& pattern,
                               const bool ignoreCase)
  {
    return impl_string::contains<char>(text, pattern, ignoreCase);
  }

  // endsWith() //////////////////////////////////////////////////////////////

  bool CS_UTIL_EXPORT endsWith(const std::string_view& text,
                               const std::string_view& pattern,
                               const bool ignoreCase)
  {
    return impl_string::endsWith<char>(text, pattern, ignoreCase);
  }

  // equals() ////////////////////////////////////////////////////////////////

  bool CS_UTIL_EXPORT equals(const std::string_view& a,
                             const std::string_view& b,
                             const bool ignoreCase)
  {
    return impl_string::equals<char>(a, b, ignoreCase);
  }

  // is...() /////////////////////////////////////////////////////////////////

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

  std::string CS_UTIL_EXPORT narrow(const std::u16string_view& text)
  {
    return impl_string::narrow<char,char16_t>(text);
  }

  std::string CS_UTIL_EXPORT narrow(const std::wstring_view& text)
  {
    return impl_string::narrow<char,wchar_t>(text);
  }

  // removeAll() /////////////////////////////////////////////////////////////

  void CS_UTIL_EXPORT removeAll(std::string& text,
                                const char pattern)
  {
    impl_string::removeAll<char>(text, std::basic_string_view(std::addressof(pattern), 1));
  }

  void CS_UTIL_EXPORT removeAll(std::string& text,
                                const std::string_view& pattern)
  {
    impl_string::removeAll<char>(text, pattern);
  }

  void CS_UTIL_EXPORT removeAll(std::wstring& text,
                                const std::wstring_view& pattern)
  {
    impl_string::removeAll<wchar_t>(text, pattern);
  }

  // removeTrailingZeros() ///////////////////////////////////////////////////

  void CS_UTIL_EXPORT removeTrailingZeros(std::string& text,
                                          const bool removeDot)
  {
    impl_string::removeTrailingZeros<char>(text, removeDot);
  }

  // replaceAll() ////////////////////////////////////////////////////////////

  void CS_UTIL_EXPORT replaceAll(std::string& str,
                                 const char before,
                                 const char after)
  {
    impl_string::replaceAll<char>(str, before, after);
  }

  void CS_UTIL_EXPORT replaceAll(std::wstring& str,
                                 const wchar_t before,
                                 const wchar_t after)
  {
    impl_string::replaceAll<wchar_t>(str, before, after);
  }

  void CS_UTIL_EXPORT replaceAll(std::string& str,
                                 const char before,
                                 const std::string_view& after)
  {
    impl_string::replaceAll<char>(str, std::string_view(std::addressof(before), 1), after);
  }

  void CS_UTIL_EXPORT replaceAll(std::string& str,
                                 const std::string_view& before,
                                 const std::string_view& after)
  {
    impl_string::replaceAll<char>(str, before, after);
  }

  void CS_UTIL_EXPORT replaceAll(std::wstring& str,
                                 const std::wstring_view& before,
                                 const std::wstring_view& after)
  {
    impl_string::replaceAll<wchar_t>(str, before, after);
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

  // startsWith() ////////////////////////////////////////////////////////////

  bool CS_UTIL_EXPORT startsWith(const std::string_view& text,
                                 const std::string_view& pattern,
                                 const bool ignoreCase)
  {
    return impl_string::startsWith<char>(text, pattern, ignoreCase);
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

  std::u16string CS_UTIL_EXPORT widenUtf16(const std::string_view& text)
  {
    return impl_string::widen<char16_t,char>(text);
  }

  std::wstring CS_UTIL_EXPORT widen(const std::string_view& text)
  {
    return impl_string::widen<wchar_t,char>(text);
  }

} // namespace cs
