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

#include <cs/Core/Container.h>
#include <cs/Core/Flags.h>
#include <cs/Text/StringReplaceImpl.h>
#include <cs/Text/StringSplitImpl.h>

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

  inline void shrink(std::string& str, const bool reclaim = false)
  {
    if( str.empty() ) {
      return;
    }

    try {
      const std::size_t len = strlen(str.data(), str.size());
      str.resize(len);
      if( reclaim ) {
        str.shrink_to_fit();
      }
    } catch( ... ) {
      str.clear();
    }
  }

  ////// String contains character/pattern/predicate... //////////////////////

  inline bool contains(const std::string_view& str,
                       const char pat)
  {
    return !str.empty()  &&  contains(str.data(), str.size(),
                                      pat);
  }

  inline bool contains(const std::string_view& str,
                       const std::string_view& pat,
                       const bool ignoreCase = false)
  {
    return !str.empty()  &&  contains(str.data(), str.size(),
                                      pat.data(), pat.size(),
                                      ignoreCase);
  }

  template<typename PredFunc>
  inline bool contains(const std::string_view& str,
                       PredFunc func,
                       if_char_predicate_t<PredFunc,char> * = nullptr)
  {
    return !str.empty()  &&  contains(str.data(), str.size(),
                                      func);
  }

  template<typename PredFunc>
  inline bool contains(const std::wstring_view& str,
                       PredFunc func,
                       if_char_predicate_t<PredFunc,wchar_t> * = nullptr)
  {
    return !str.empty()  &&  contains(str.data(), str.size(),
                                      func);
  }

  ////// String ends with pattern... /////////////////////////////////////////

  inline bool endsWith(const std::string_view& str,
                       const std::string_view& pat,
                       const bool ignoreCase = false)
  {
    return !str.empty()  &&  endsWith(str.data(), str.size(),
                                      pat.data(), pat.size(),
                                      ignoreCase);
  }

  ////// Strings are equal... ////////////////////////////////////////////////

  inline bool equals(const std::string_view& a,
                     const std::string_view& b,
                     const bool ignoreCase = false)
  {
    return !a.empty()  &&  equals(a.data(), a.size(),
                                  b.data(), b.size(),
                                  ignoreCase);
  }

  ////// String classification... ////////////////////////////////////////////

  inline bool isHexString(const std::string_view& str)
  {
    return !str.empty()  &&  isHexString(str.data(), str.size());
  }

  inline bool isIdent(const std::string_view& str)
  {
    return !str.empty()  &&  isIdent(str.data(), str.size());
  }

  inline bool isSpace(const std::string_view& str)
  {
    return !str.empty()  &&  isSpace(str.data(), str.size());
  }

  ////// Remove character/pattern/predicate from string... ///////////////////

  inline void removeAll(std::string& str,
                        const char pat)
  {
    if( !str.empty() ) {
      removeAll(str.data(), str.size(),
                pat);
      shrink(str);
    }
  }

  inline void removeAll(std::string& str,
                        const std::string_view& pat)
  {
    if( !str.empty() ) {
      removeAll(str.data(), str.size(),
                pat.data(), pat.size());
      shrink(str);
    }
  }

  template<typename PredFunc>
  inline void removeAll(std::string& str,
                        PredFunc func,
                        if_char_predicate_t<PredFunc,char> * = nullptr)
  {
    if( !str.empty() ) {
      removeAll(str.data(), str.size(),
                func);
      shrink(str);
    }
  }

  ////// Remove Trailing Zeros from Fixed-Notation Floating-Point String... //

  inline void removeTrailingZeros(std::string& str,
                                  const bool removeDot = true)
  {
    if( !str.empty() ) {
      removeTrailingZeros(str.data(), str.size(),
                          removeDot);
      shrink(str);
    }
  }

  ////// Replace character/predicate with character... ///////////////////////

  inline void replaceAll(std::string& str,
                         const char pat,
                         const char txt)
  {
    if( !str.empty() ) {
      replaceAll(str.data(), str.size(),
                 pat,
                 txt);
    }
  }

  inline void replaceAll(std::wstring& str,
                         const wchar_t pat,
                         const wchar_t txt)
  {
    if( !str.empty() ) {
      replaceAll(str.data(), str.size(),
                 pat,
                 txt);
    }
  }

  template<typename PredFunc>
  inline void replaceAll(std::string& str,
                         PredFunc func,
                         const char txt,
                         if_char_predicate_t<PredFunc,char> * = nullptr)
  {
    if( !str.empty() ) {
      replaceAll(str.data(), str.size(),
                 func,
                 txt);
    }
  }

  ////// Replace character/pattern with text... //////////////////////////////

  inline void replaceAll(std::string& str,
                         const char pat,
                         const std::string_view& txt)
  {
    if( !str.empty()  &&
        !txt.empty() ) {
      impl_string::replaceAll(str,
                              &pat, 1,
                              txt.data(), txt.size());
    }
  }

  inline void replaceAll(std::string& str,
                         const std::string_view& pat,
                         const std::string_view& txt)
  {
    if( !str.empty()  &&
        str.size() >= pat.size()  &&  !txt.empty() ) {
      impl_string::replaceAll(str,
                              pat.data(), pat.size(),
                              txt.data(), txt.size());
    }
  }

  ////// Replace consecutive whitespace with single space... /////////////////

  inline void simplify(std::string& str)
  {
    if( !str.empty() ) {
      simplify(str.data(), str.size());
      shrink(str);
    }
  }

  inline std::string toSimplified(std::string str)
  {
    simplify(str);
    return str;
  }

  ////// Split string at character/pattern... ////////////////////////////////

  inline std::list<std::string> split(const std::string_view& str,
                                      const char pat,
                                      const SplitFlags flags = SplitFlag::None)
  {
    if( !str.empty() ) {
      return impl_string::split(str.data(), str.data() + str.size(),
                                pat,
                                flags.testAny(SplitFlag::SkipEmpty),
                                flags.testAny(SplitFlag::Trim));
    }
    return std::list<std::string>();
  }

  inline std::list<std::string> split(const std::string_view& str,
                                      const std::string_view& pat,
                                      const SplitFlags flags = SplitFlag::None)
  {
    if( !str.empty()  &&  str.size() >= pat.size() ) {
      return impl_string::split(str.data(), str.data() + str.size(),
                                pat.data(), pat.size(),
                                flags.testAny(SplitFlag::SkipEmpty),
                                flags.testAny(SplitFlag::Trim));
    }
    return std::list<std::string>();
  }

  ////// String starts with pattern... ///////////////////////////////////////

  inline bool startsWith(const std::string_view& str,
                         const std::string_view& pat,
                         const bool ignoreCase = false)
  {
    return !str.empty()  &&  startsWith(str.data(), str.size(),
                                        pat.data(), pat.size(),
                                        ignoreCase);
  }

  ////// Case conversion... //////////////////////////////////////////////////

  inline void lower(std::string& str)
  {
    if( !str.empty() ) {
      toLower(str.data(), str.size());
    }
  }

  inline std::string toLower(std::string str)
  {
    lower(str);
    return str;
  }

  inline void upper(std::string& str)
  {
    if( !str.empty() ) {
      toUpper(str.data(), str.size());
    }
  }

  inline std::string toUpper(std::string str)
  {
    upper(str);
    return str;
  }

  ////// Remove whitespace from begin & end... ///////////////////////////////

  inline void trim(std::string& str)
  {
    if( !str.empty() ) {
      trim(str.data(), str.size());
      shrink(str);
    }
  }

  inline std::string toTrimmed(std::string str)
  {
    trim(str);
    return str;
  }

  ////// Narrowing/Widening of strings... ////////////////////////////////////

  inline std::string toString(const std::u16string_view& str)
  {
    std::string result;
    if( str.empty()  ||  !resize(result, str.size()) ) {
      return std::string();
    }

    impl_string::narrow(result.data(), str.data(), str.data() + str.size());

    return result;
  }

  inline std::string toString(const std::wstring_view& str)
  {
    std::string result;
    if( str.empty()  ||  !resize(result, str.size()) ) {
      return std::string();
    }

    impl_string::narrow(result.data(), str.data(), str.data() + str.size());

    return result;
  }

  inline std::u16string toUtf16String(const std::string_view& str)
  {
    std::u16string result;
    if( str.empty()  ||  !resize(result, str.size()) ) {
      return std::u16string();
    }

    impl_string::widen(result.data(), str.data(), str.data() + str.size());

    return result;
  }

  inline std::wstring toWString(const std::string_view& str)
  {
    std::wstring result;
    if( str.empty()  ||  !resize(result, str.size()) ) {
      return std::wstring();
    }

    impl_string::widen(result.data(), str.data(), str.data() + str.size());

    return result;
  }

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

  inline const wchar_t *WSTR(const std::u16string& str,
                             std::enable_if_t<sizeof(wchar_t) == sizeof(char16_t)> * = nullptr)
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

  inline std::u16string toUtf16String(const std::wstring_view& str,
                                      std::enable_if_t<sizeof(char16_t) == sizeof(wchar_t)> * = nullptr)
  {
    return !str.empty()
        ? std::u16string(UTF16(str.data()), str.size())
        : std::u16string();
  }

  inline std::wstring toWString(const std::u16string_view& str,
                                std::enable_if_t<sizeof(wchar_t) == sizeof(char16_t)> * = nullptr)
  {
    return !str.empty()
        ? std::wstring(WSTR(str.data()), str.size())
        : std::wstring();
  }

} // namespace cs
