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
#include <string>

#include <cs/Text/StringReplaceImpl.h>
#include <cs/Text/StringSplitImpl.h>

namespace cs {

  ////// Types ///////////////////////////////////////////////////////////////

  template<typename T> requires is_char_v<T>
  using String = std::basic_string<T>;

  template<typename T> requires is_char_v<T>
  using StringIter = typename String<T>::iterator;

  template<typename T> requires is_char_v<T>
  using ConstStringIter = typename String<T>::const_iterator;

  template<typename T> requires is_char_v<T>
  using StringList = std::list<String<T>>;

  template<typename T> requires is_char_v<T>
  using StringListIter = typename StringList<T>::iterator;

  template<typename T> requires is_char_v<T>
  using ConstStringListIter = typename StringList<T>::const_iterator;

  template<typename T> requires is_char_v<T>
  using StringView = std::basic_string_view<T>;

  ////// Length Functions ////////////////////////////////////////////////////

  inline void shrink(std::string *str, const bool reclaim = false)
  {
    str->resize(strlen(str->data(), str->size()));
    if( reclaim ) {
      str->shrink_to_fit();
    }
  }

  inline std::size_t strlen(const std::string_view& str)
  {
    return strlen(str.data(), str.size());
  }

  ////// String contains character... ////////////////////////////////////////

  inline bool contains(const std::string_view& str,
                       const char& pat)
  {
    return !str.empty()  &&  contains(str.data(), str.size(),
                                      pat);
  }

  ////// String contains predicate... ////////////////////////////////////////

  template<typename PredFunc>
  inline bool contains(const std::string_view& str,
                       PredFunc func,
                       if_char_predicate_t<PredFunc,char> * = nullptr)
  {
    return !str.empty()  &&  contains(str.data(), str.size(),
                                      func);
  }

  ////// String contains pattern... //////////////////////////////////////////

  inline bool contains(const std::string_view& str,
                       const std::string_view& pat,
                       const bool ignoreCase = false)
  {
    return !str.empty()  &&  str.size() >= pat.size()  &&  contains(str.data(), str.size(),
                                                                    pat.data(), pat.size(),
                                                                    ignoreCase);
  }

  ////// String ends with pattern... /////////////////////////////////////////

  inline bool endsWith(const std::string_view& str,
                       const std::string_view& pat,
                       const bool ignoreCase = false)
  {
    return !str.empty()  &&  str.size() >= pat.size()  &&  endsWith(str.data(), str.size(),
                                                                    pat.data(), pat.size(),
                                                                    ignoreCase);
  }

  ////// Strings are equal... ////////////////////////////////////////////////

  inline bool equals(const std::string_view& a,
                     const std::string_view& b,
                     const bool ignoreCase = false)
  {
    return !a.empty()  &&  !b.empty()  &&  equals(a.data(), a.size(),
                                                  b.data(), b.size(),
                                                  ignoreCase);
  }

  ////// String is hexadecimal string... /////////////////////////////////////

  inline bool isHexString(const std::string_view& str)
  {
    return !str.empty()  &&  isHexString(str.data(), str.size());
  }

  ////// String is C-style identifier... /////////////////////////////////////

  inline bool isIdent(const std::string_view& str)
  {
    return !str.empty()  &&  isIdent(str.data(), str.size());
  }

  ////// String contains only whitespace... //////////////////////////////////

  inline bool isSpace(const std::string_view& str)
  {
    return !str.empty()  &&  isSpace(str.data(), str.size());
  }

  ////// Remove pattern from string... ///////////////////////////////////////

  inline void removeAll(std::string *str,
                        const std::string_view& pat)
  {
    if( str != nullptr  &&  !str->empty()  &&  str->size() >= pat.size() ) {
      removeAll(str->data(), str->size(),
                pat.data(), pat.size());
      shrink(str);
    }
  }

  ////// Remove character from string... /////////////////////////////////////

  inline void removeAll(std::string *str,
                        const char& pat)
  {
    if( str != nullptr  &&  !str->empty() ) {
      removeAll(str->data(), str->size(),
                pat);
      shrink(str);
    }
  }

  ////// Remove character matching predicate from string... //////////////////

  template<typename PredFunc>
  inline void removeAll(std::string *str,
                        PredFunc func,
                        if_char_predicate_t<PredFunc,char> * = nullptr)
  {
    if( str != nullptr  &&  !str->empty() ) {
      removeAll(str->data(), str->size(),
                func);
      shrink(str);
    }
  }

  ////// Remove Trailing Zeros from Fixed-Notation Floating-Point String /////

  inline void removeTrailingZeros(std::string *str,
                                  const bool removeDot = true)
  {
    if( str != nullptr  &&  !str->empty() ) {
      removeTrailingZeros(str->data(), str->size(),
                          removeDot);
      shrink(str);
    }
  }

  ////// Replace pattern in string... ////////////////////////////////////////

  inline void replaceAll(std::string *str,
                         const char& pat,
                         const std::string_view& txt)
  {
    if( str != nullptr  &&  !str->empty()  &&
        !txt.empty() ) {
      impl_string::replaceAll(str,
                              &pat, 1,
                              txt.data(), txt.size());
    }
  }

  inline void replaceAll(std::string *str,
                         const std::string_view& pat,
                         const std::string_view& txt)
  {
    if( str != nullptr  &&  !str->empty()  &&
        str->size() >= pat.size()  &&  !txt.empty() ) {
      impl_string::replaceAll(str,
                              pat.data(), pat.size(),
                              txt.data(), txt.size());
    }
  }

  ////// Replace character in string... //////////////////////////////////////

  inline void replaceAll(std::string *str,
                         const char& pat,
                         const char& txt)
  {
    if( str != nullptr  &&  !str->empty() ) {
      replaceAll(str->data(), str->size(),
                 pat,
                 txt);
    }
  }

  ////// Replace character matching predicate in string... ///////////////////

  template<typename PredFunc>
  inline void replaceAll(std::string *str,
                         PredFunc func,
                         const char& txt,
                         if_char_predicate_t<PredFunc,char> * = nullptr)
  {
    if( str != nullptr  &&  !str->empty() ) {
      replaceAll(str->data(), str->size(),
                 func,
                 txt);
    }
  }

  ////// Replace consecutive whitespace with single space... /////////////////

  inline void simplify(std::string *str)
  {
    if( str != nullptr  &&  !str->empty() ) {
      simplify(str->data(), str->size());
      shrink(str);
    }
  }

  inline std::string simplified(std::string str)
  {
    simplify(&str);
    return str;
  }

  ////// Split string at pattern... //////////////////////////////////////////

  inline StringList<char> split(const std::string_view& str,
                                const std::string_view& pat,
                                const bool skipEmpty = false, const bool doTrim = false)
  {
    if( !str.empty()  &&  str.size() >= pat.size() ) {
      return impl_string::split(str.data(), str.data() + str.size(),
                                pat.data(), pat.size(),
                                skipEmpty, doTrim);
    }
    return StringList<char>();
  }

  ////// Split string at character... ////////////////////////////////////////

  inline StringList<char> split(const std::string_view& str,
                                const char& pat,
                                const bool skipEmpty = false, const bool doTrim = false)
  {
    if( !str.empty() ) {
      return impl_string::split(str.data(), str.data() + str.size(),
                                pat,
                                skipEmpty, doTrim);
    }
    return StringList<char>();
  }

  ////// String starts with pattern... ///////////////////////////////////////

  inline bool startsWith(const std::string_view& a,
                         const std::string_view& b,
                         const bool ignoreCase = false)
  {
    return !a.empty()  &&  !b.empty()  &&  equals(a.data(), a.size(),
                                                  b.data(), b.size(),
                                                  ignoreCase);
  }

  ////// Case conversion... //////////////////////////////////////////////////

  inline void toLower(std::string *str)
  {
    if( str != nullptr  &&  !str->empty() ) {
      toLower(str->data(), str->size());
    }
  }

  inline std::string toLower(std::string str)
  {
    toLower(&str);
    return str;
  }

  inline void toUpper(std::string *str)
  {
    if( str != nullptr  &&  !str->empty() ) {
      toUpper(str->data(), str->size());
    }
  }

  inline std::string toUpper(std::string str)
  {
    toUpper(&str);
    return str;
  }

  ////// Remove whitespace from begin & end... ///////////////////////////////

  inline void trim(std::string *str)
  {
    if( str != nullptr  &&  !str->empty() ) {
      trim(str->data(), str->size());
      shrink(str);
    }
  }

  inline std::string trimmed(std::string str)
  {
    trim(&str);
    return str;
  }

  ////// Type conversion... //////////////////////////////////////////////////

  inline std::string toString(const std::u8string& str)
  {
    return !str.empty()
        ? std::string(CSTR(str.data()), str.size())
        : std::string();
  }

  inline std::u8string toUtf8String(const std::string& str)
  {
    return !str.empty()
        ? std::u8string(UTF8(str.data()), str.size())
        : std::u8string();
  }

} // namespace cs
