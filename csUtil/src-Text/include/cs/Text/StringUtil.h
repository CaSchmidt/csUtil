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

#include <cs/Core/csutil_config.h>

#include <cs/Core/Flags.h>
#include <cs/Text/StringQueryImpl.h>
#include <cs/Text/StringRemoveImpl.h>
#include <cs/Text/StringReplaceImpl.h>

namespace cs {

  ////// Flags ///////////////////////////////////////////////////////////////

  enum class SplitFlag : unsigned {
    None      = 0,
    SkipEmpty = 0x01,
    Trim      = 0x02,
    All       = 0xFF
  };

} // namespace cs

CS_ENABLE_FLAGS(cs::SplitFlag)

namespace cs {

  ////// Types ///////////////////////////////////////////////////////////////

  using SplitFlags = Flags<SplitFlag>;

  ////// Length Functions ////////////////////////////////////////////////////

  void CS_UTIL_EXPORT shrink(std::string& str, const bool reclaim = false);

  ////// String contains character/pattern/predicate... //////////////////////

  bool CS_UTIL_EXPORT contains(const std::string_view& text,
                               const char pattern);

  bool CS_UTIL_EXPORT contains(const std::string_view& text,
                               const std::string_view& pattern,
                               const bool ignoreCase = false);

  template<typename UnaryPred>
  inline bool contains(const std::string_view& text,
                       UnaryPred pred,
                       if_unary_predicate_t<UnaryPred,char> * = nullptr)
  {
    return impl_string::contains<char,UnaryPred>(text, pred);
  }

  template<typename UnaryPred>
  inline bool contains(const std::wstring_view& text,
                       UnaryPred pred,
                       if_unary_predicate_t<UnaryPred,wchar_t> * = nullptr)
  {
    return impl_string::contains<wchar_t,UnaryPred>(text, pred);
  }

  ////// String ends with pattern... /////////////////////////////////////////

  bool CS_UTIL_EXPORT endsWith(const std::string_view& text,
                               const std::string_view& pattern,
                               const bool ignoreCase = false);

  ////// Strings are equal... ////////////////////////////////////////////////

  bool CS_UTIL_EXPORT equals(const std::string_view& a,
                             const std::string_view& b,
                             const bool ignoreCase = false);

  ////// String classification... ////////////////////////////////////////////

  bool CS_UTIL_EXPORT isHexString(const std::string_view& text);

  bool CS_UTIL_EXPORT isIdent(const std::string_view& text);

  bool CS_UTIL_EXPORT isSpace(const std::string_view& text);

  ////// Join string list... /////////////////////////////////////////////////

  std::string CS_UTIL_EXPORT join(const std::list<std::string>& list,
                                  const char pattern = '\0');

  std::string CS_UTIL_EXPORT join(const std::list<std::string>& list,
                                  const std::string_view& pattern);

  ////// Remove character/pattern/predicate from string... ///////////////////

  void CS_UTIL_EXPORT removeAll(std::string& text,
                                const char pattern);

  void CS_UTIL_EXPORT removeAll(std::string& text,
                                const std::string_view& pattern);

  void CS_UTIL_EXPORT removeAll(std::wstring& text,
                                const std::wstring_view& pattern);

  template<typename UnaryPred>
  inline void removeAll(std::string& text,
                        UnaryPred pred,
                        if_unary_predicate_t<UnaryPred,char> * = nullptr)
  {
    impl_string::removeAll<char,UnaryPred>(text, pred);
  }

  ////// Remove Trailing Zeros from Fixed-Notation Floating-Point String... //

  void CS_UTIL_EXPORT removeTrailingZeros(std::string& text,
                                          const bool removeDot = true);

  ////// Replace character/predicate with character... ///////////////////////

  void CS_UTIL_EXPORT replaceAll(std::string& str,
                                 const char before,
                                 const char after);

  void CS_UTIL_EXPORT replaceAll(std::wstring& str,
                                 const wchar_t before,
                                 const wchar_t after);

  template<typename UnaryPred>
  inline void replaceAll(std::string& str,
                         UnaryPred pred,
                         const char after)
  {
    impl_string::replaceAll<char,UnaryPred>(str, pred, after);
  }

  ////// Replace character/pattern with text... //////////////////////////////

  void CS_UTIL_EXPORT replaceAll(std::string& str,
                                 const char before,
                                 const std::string_view& after);

  void CS_UTIL_EXPORT replaceAll(std::string& str,
                                 const std::string_view& before,
                                 const std::string_view& after);

  void CS_UTIL_EXPORT replaceAll(std::wstring& str,
                                 const std::wstring_view& before,
                                 const std::wstring_view& after);

  ////// Replace consecutive whitespace with single space... /////////////////

  void CS_UTIL_EXPORT simplify(std::string& text);

  std::string CS_UTIL_EXPORT toSimplified(std::string text);

  ////// Split string at character/pattern... ////////////////////////////////

  std::list<std::string> CS_UTIL_EXPORT split(const std::string_view& text,
                                              const char pattern,
                                              const SplitFlags flags = SplitFlag::None);

  std::list<std::string> CS_UTIL_EXPORT split(const std::string_view& text,
                                              const std::string_view& pattern,
                                              const SplitFlags flags = SplitFlag::None);

  ////// String starts with pattern... ///////////////////////////////////////

  bool CS_UTIL_EXPORT startsWith(const std::string_view& text,
                                 const std::string_view& pattern,
                                 const bool ignoreCase = false);

  ////// Case conversion... //////////////////////////////////////////////////

  void CS_UTIL_EXPORT lower(std::string& text);

  std::string CS_UTIL_EXPORT toLower(std::string text);

  void CS_UTIL_EXPORT upper(std::string& text);

  std::string CS_UTIL_EXPORT toUpper(std::string text);

  ////// Remove whitespace from begin & end... ///////////////////////////////

  void CS_UTIL_EXPORT trim(std::string& text);

  std::string CS_UTIL_EXPORT toTrimmed(std::string text);

  ////// Narrowing/Widening of strings... ////////////////////////////////////

  std::string CS_UTIL_EXPORT narrow(const std::u16string_view& text);

  std::string CS_UTIL_EXPORT narrow(const std::wstring_view& text);

  std::u16string CS_UTIL_EXPORT widenUtf16(const std::string_view& text);

  std::wstring CS_UTIL_EXPORT widen(const std::string_view& text);

  ////// Type conversion... //////////////////////////////////////////////////

  /*
   * NOTE: CSTR() and WSTR() cannot operate on std::basic_string_view<> as
   * it is not required to be null-terminated!
   *
   * Using these functions with std::basic_string_view<> will copy the
   * view to a std::basic_string<>.
   */

  inline const char *CSTR(const std::u8string& str)
  {
    return !str.empty()
        ? CSTR(str.data())
        : nullptr;
  }

  inline const wchar_t *WSTR(const std::u16string& str)
  {
    return !str.empty()
        ? WSTR(str.data())
        : nullptr;
  }

  inline std::string toString(const std::u8string_view& str)
  {
    return !str.empty()
        ? std::string(CSTR(str.data()), str.size())
        : std::string();
  }

  inline std::u8string toUtf8String(const std::string_view& str)
  {
    return !str.empty()
        ? std::u8string(UTF8(str.data()), str.size())
        : std::u8string();
  }

  inline std::u8string_view toUtf8StringView(const std::string_view& str)
  {
    return !str.empty()
        ? std::u8string_view(UTF8(str.data()), str.size())
        : std::u8string_view();
  }

  inline std::u16string toUtf16String(const std::wstring_view& str)
  {
    return !str.empty()
        ? std::u16string(UTF16(str.data()), str.size())
        : std::u16string();
  }

  inline std::wstring toWString(const std::u16string_view& str)
  {
    return !str.empty()
        ? std::wstring(WSTR(str.data()), str.size())
        : std::wstring();
  }

} // namespace cs
