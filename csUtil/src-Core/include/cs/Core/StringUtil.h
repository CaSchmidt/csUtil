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

#include <algorithm>
#include <iterator>
#include <list>
#include <string>

#include <cs/Core/CharUtil.h>

namespace cs {

  ////// Types ///////////////////////////////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  using String = std::basic_string<T>;

  template<typename T> requires IsCharacter<T>
  using StringIter = typename String<T>::iterator;

  template<typename T> requires IsCharacter<T>
  using ConstStringIter = typename String<T>::const_iterator;

  template<typename T> requires IsCharacter<T>
  using StringList = std::list<String<T>>;

  template<typename T> requires IsCharacter<T>
  using StringListIter = typename StringList<T>::iterator;

  template<typename T> requires IsCharacter<T>
  using ConstStringListIter = typename StringList<T>::const_iterator;

  ////// Constants ///////////////////////////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  inline constexpr typename String<T>::size_type NPOS = String<T>::npos;

  ////// Length Functions ////////////////////////////////////////////////////

  namespace impl_str {

    template<typename T> requires IsCharacter<T>
    constexpr std::size_t length2(const T *str)
    {
      return *str == glyph<T>::null
          ? 0
          : 1 + length2(str + 1);
    }

  } // namespace impl_str

  template<typename T> requires IsCharacter<T>
  constexpr std::size_t length(const T *str) noexcept
  {
    return str == nullptr
        ? 0
        : impl_str::length2(str);
  }

  template<typename T> requires IsCharacter<T>
  inline std::size_t lengthRange(const T *first, const T *last) noexcept
  {
    return first != nullptr  &&  first < last
        ? std::distance(first, std::find(first, last, glyph<T>::null))
        : 0;
  }

  template<typename T> requires IsCharacter<T>
  inline std::size_t lengthRange(const T *str, const std::size_t len) noexcept
  {
    return lengthRange(str, str + len);
  }

  template<typename T> requires IsCharacter<T>
  inline std::size_t lengthRange(const String<T>& str)
  {
    return lengthRange(str.data(), str.size());
  }

  ////// String ends with pattern... /////////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  inline bool endsWith(const T *str, const std::size_t lenstr,
                       const T *pat, const std::size_t lenpat,
                       const bool ignoreCase = false) noexcept
  {
    const std::size_t maxstr = lenstr == MAX_SIZE_T
        ? length(str)
        : lenstr;
    const std::size_t maxpat = lenpat == MAX_SIZE_T
        ? length(pat)
        : lenpat;

    if( str == nullptr  ||  pat == nullptr  ||
        maxstr < 1  ||  maxpat < 1  ||  maxstr < maxpat ) {
      return false;
    }

    return ignoreCase
        ? std::equal(str + maxstr - maxpat, str + maxstr, pat, lambda_eqI<T>())
        : std::equal(str + maxstr - maxpat, str + maxstr, pat);
  }

  template<typename T> requires IsCharacter<T>
  inline bool endsWith(const T *str, const T *pat, const bool ignoreCase = false)
  {
    return endsWith<T>(str, MAX_SIZE_T, pat, MAX_SIZE_T, ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool endsWith(const String<T>& str, const T *pat, const bool ignoreCase = false)
  {
    return endsWith<T>(str.data(), str.size(), pat, MAX_SIZE_T, ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool endsWith(const T *str, const String<T>& pat, const bool ignoreCase = false)
  {
    return endsWith<T>(str, MAX_SIZE_T, pat.data(), pat.size(), ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool endsWith(const String<T>& str, const String<T>& pat, const bool ignoreCase = false)
  {
    return endsWith<T>(str.data(), str.size(), pat.data(), pat.size(), ignoreCase);
  }

  ////// Strings are equal... ////////////////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  inline bool equalsN(const T *a, const T *b, const std::size_t len,
                      const bool ignoreCase = false) noexcept
  {
    const std::size_t max = len == MAX_SIZE_T
        ? std::min(length(a), length(b))
        : len;

    if( a == nullptr  ||  b == nullptr  ||
        max < 1 ) {
      return false;
    }

    return ignoreCase
        ? std::equal(a, a + max, b, lambda_eqI<T>())
        : std::equal(a, a + max, b);
  }

  template<typename T> requires IsCharacter<T>
  inline bool equalsN(const T *a, const T *b, const bool ignoreCase = false)
  {
    return equalsN<T>(a, b, MAX_SIZE_T, ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool equals(const T *a, const std::size_t lena,
                     const T *b, const std::size_t lenb,
                     const bool ignoreCase = false)
  {
    const std::size_t maxa = lena == MAX_SIZE_T
        ? length(a)
        : lena;
    const std::size_t maxb = lenb == MAX_SIZE_T
        ? length(b)
        : lenb;

    if( a == nullptr  ||  b == nullptr  ||
        maxa < 1  ||  maxb < 1  ||  maxa != maxb ) {
      return false;
    }

    return ignoreCase
        ? std::equal(a, a + maxa, b, lambda_eqI<T>())
        : std::equal(a, a + maxa, b);
  }

  template<typename T> requires IsCharacter<T>
  inline bool equals(const T *a, const T *b, const bool ignoreCase = false)
  {
    return equals<T>(a, MAX_SIZE_T, b, MAX_SIZE_T, ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool equals(const String<T>& a, const T *b, const bool ignoreCase = false)
  {
    return equals<T>(a.data(), a.size(), b, MAX_SIZE_T, ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool equals(const T *a, const String<T>& b, const bool ignoreCase = false)
  {
    return equals<T>(a, MAX_SIZE_T, b.data(), b.size(), ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool equals(const String<T>& a, const String<T>& b, const bool ignoreCase = false)
  {
    return equals<T>(a.data(), a.size(), b.data(), b.size(), ignoreCase);
  }

  ////// String is C-style identifier... /////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  inline bool isIdent(const T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = len == MAX_SIZE_T
        ? length(str)
        : len;

    if( str == nullptr  ||
        max < 1 ) {
      return false;
    }

    const T *first = str;
    const T  *last = first + max;

    if( !isIdentFirst(*first) ) {
      return false;
    }

    return std::all_of(++first, last, lambda_is_ident<T>());
  }

  template<typename T> requires IsCharacter<T>
  inline bool isIdent(const String<T>& str)
  {
    return isIdent<T>(str.data(), str.size());
  }

  ////// String contains only whitespace... //////////////////////////////////

  template<typename T> requires IsCharacter<T>
  inline bool isSpace(const T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = len == MAX_SIZE_T
        ? length(str)
        : len;

    if( str == nullptr  ||
        max < 1 ) {
      return false;
    }

    return std::all_of(str, str + max, lambda_is_space<T>());
  }

  template<typename T> requires IsCharacter<T>
  inline bool isSpace(const String<T>& str)
  {
    return isSpace<T>(str.data(), str.size());
  }

  ////// Remove pattern from string... ///////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  inline void removeAll(String<T> *str, const T *pat, const std::size_t lenpat = MAX_SIZE_T)
  {
    const std::size_t sizpat = lenpat == MAX_SIZE_T
        ? length(pat)
        : lenpat;

    if( str->size() < 1  ||  pat == nullptr  ||
        sizpat < 1  ||  str->size() < sizpat ) {
      return;
    }

    for(std::size_t pos = 0; (pos = str->find(pat, pos, sizpat)) != NPOS<T>; ) {
      str->erase(pos, sizpat);
    }
  }

  template<typename T> requires IsCharacter<T>
  inline void removeAll(String<T> *str, const String<T>& pat)
  {
    removeAll<T>(str, pat.data(), pat.size());
  }

  template<typename T> requires IsCharacter<T>
  inline void removeAll(String<T> *str, const T& pat)
  {
    removeAll<T>(str, &pat, 1);
  }

  template<typename T, typename PredFunc> requires IsCharacter<T>
  inline void removeAll(T *first, T *last, PredFunc func)
  {
    T *end = std::remove_if(first, last, func);
    std::for_each(end, last, lambda_set_null<T>());
  }

  template<typename T, typename PredFunc> requires IsCharacter<T>
  inline void removeAll(String<T> *str, PredFunc func)
  {
    StringIter<T> end = std::remove_if(str->begin(), str->end(), func);
    str->erase(end, str->end());
  }

  ////// Remove Trailing Zeros from Fixed-Notation Floating-Point String /////

  template<typename T> requires IsCharacter<T>
  inline void removeTrailingZeros(T *first, T *last,
                                  const bool removeDot = true)
  {
    using RevIter = std::reverse_iterator<T*>;

    using g = glyph<T>;

    // (0) Sanity Check //////////////////////////////////////////////////////

    if( first == nullptr  ||  last <= first ) {
      return;
    }

    // (1) Find '.' //////////////////////////////////////////////////////////

    T *it_dot = std::find(first, last, g::dot);
    if( it_dot == last ) { // Nothing to do!
      return;
    }

    // (2) Do not touch exponent notation! ///////////////////////////////////

    if( std::find(it_dot + 1, last, g::e) != last ) {
      return;
    }

    if( std::find(it_dot + 1, last, g::E) != last ) {
      return;
    }

    // (3) Remove Trailing Zeros /////////////////////////////////////////////

    T *it_trailing = std::find_if_not(RevIter(last), RevIter(it_dot),
                                      lambda_is_zero<T>()).base();
    std::for_each(it_trailing, last, lambda_set_null<T>());

    if( removeDot  &&  it_trailing == it_dot + 1 ) {
      *it_dot = g::null;
    }
  }

  template<typename T> requires IsCharacter<T>
  inline void removeTrailingZeros(T *str, const std::size_t len = MAX_SIZE_T,
                                  const bool removeDot = true)
  {
    const std::size_t max = len == MAX_SIZE_T
        ? length(str)
        : lengthRange(str, len);
    removeTrailingZeros(str, str + max, removeDot);
  }

  template<typename T> requires IsCharacter<T>
  inline void removeTrailingZeros(String<T> *str, const bool removeDot = true)
  {
    removeTrailingZeros(str->data(), str->size(), removeDot);
    str->resize(lengthRange(*str));
  }

  ////// Replace pattern in string... ////////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  inline void replaceAll(String<T> *str,
                         const T *pat, const std::size_t lenpat,
                         const T *txt, const std::size_t lentxt = MAX_SIZE_T)
  {
    const std::size_t sizpat = lenpat == MAX_SIZE_T
        ? length(pat)
        : lenpat;
    const std::size_t siztxt = lentxt == MAX_SIZE_T
        ? length(txt)
        : lentxt;

    if( str->size() < 1  ||  pat == nullptr  ||  txt == nullptr  ||
        sizpat < 1  ||  siztxt < 1  ||  str->size() < sizpat ) {
      return;
    }

    for(std::size_t pos = 0; (pos = str->find(pat, pos, sizpat)) != NPOS<T>; pos += siztxt) {
      str->replace(pos, sizpat, txt, siztxt);
    }
  }

  template<typename T> requires IsCharacter<T>
  inline void replaceAll(String<T> *str, const T *pat, const T *txt)
  {
    replaceAll<T>(str, pat, MAX_SIZE_T, txt, MAX_SIZE_T);
  }

  template<typename T> requires IsCharacter<T>
  inline void replaceAll(String<T> *str, const String<T>& pat, const T *txt)
  {
    replaceAll<T>(str, pat.data(), pat.size(), txt, MAX_SIZE_T);
  }

  template<typename T> requires IsCharacter<T>
  inline void replaceAll(String<T> *str, const T *pat, const String<T>& txt)
  {
    replaceAll<T>(str, pat, MAX_SIZE_T, txt.data(), txt.size());
  }

  template<typename T> requires IsCharacter<T>
  inline void replaceAll(String<T> *str, const String<T>& pat, const String<T>& txt)
  {
    replaceAll<T>(str, pat.data(), pat.size(), txt.data(), txt.size());
  }

  template<typename T> requires IsCharacter<T>
  inline void replaceAll(String<T> *str, const T& pat, const char *txt)
  {
    replaceAll<T>(str, &pat, 1, txt, MAX_SIZE_T);
  }

  template<typename T> requires IsCharacter<T>
  inline void replaceAll(String<T> *str, const T& pat, const String<T>& txt)
  {
    replaceAll<T>(str, &pat, 1, txt.data(), txt.size());
  }

  ////// Reclaim memory... ///////////////////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  inline void shrink(String<T> *str)
  {
    str->resize(lengthRange(*str));
    str->shrink_to_fit();
  }

  ////// Remove whitespace from begin & end... ///////////////////////////////

  template<typename T> requires IsCharacter<T>
  inline String<T> trimmed(String<T> str) noexcept
  {
    // trim left
    str.erase(str.begin(),
              std::find_if(str.begin(), str.end(), lambda_is_not_space<T>()));
    // trim right
    str.erase(std::find_if(str.rbegin(), str.rend(), lambda_is_not_space<T>()).base(),
              str.end());
    return str;
  }

  ////// Replace consecutive whitespace with single space... /////////////////

  template<typename T> requires IsCharacter<T>
  inline String<T> simplified(String<T> str) noexcept
  {
    constexpr auto lambda_adjacent_space = [](const T& a, const T& b) -> bool {
      return isSpace(a)  &&  isSpace(b);
    };

    // (1) remove duplicate whitespace
    ConstStringIter<T> last = std::unique(str.begin(), str.end(), lambda_adjacent_space);
    str.erase(last, str.end());
    // (2) replace single whitespace characters with space
    std::replace_if(str.begin(), str.end(), lambda_is_space<T>(), glyph<T>::space);
    // (3) return trimmed result
    return trimmed(str);
  }

  ////// Split string at delimiter... ////////////////////////////////////////

  namespace impl_str {

    template<typename T> requires IsCharacter<T>
    inline void extract(StringList<T>& result, const String<T>& txt,
                        const std::size_t first, const std::size_t last,
                        const bool skipEmpty, const bool doTrim)
    {
      const std::size_t len = last - first;
      String<T> part = len > 0
          ? txt.substr(first, len)
          : String<T>();

      if( skipEmpty  &&  part.empty() ) {
        return;
      }

      if( doTrim ) {
        part = trimmed(part);
      }

      result.push_back(std::move(part));
    }

  } // namespace impl_str

  template<typename T> requires IsCharacter<T>
  inline StringList<T> split(const String<T>& txt,
                             const T *del, const std::size_t lendel,
                             const bool skipEmpty = false, const bool doTrim = false)
  {
    const std::size_t maxdel = lendel == MAX_SIZE_T
        ? length(del)
        : lendel;

    if( txt.size() < 1  ||  del == nullptr  ||
        maxdel < 1  ||  txt.size() < maxdel ) {
      return StringList<T>();
    }

    StringList<T> result;

    std::size_t pos = 0;
    for(std::size_t hit; (hit = txt.find(del, pos, maxdel)) != NPOS<T>; pos = hit + maxdel) {
      impl_str::extract(result, txt, pos, hit, skipEmpty, doTrim);
    }
    impl_str::extract(result, txt, pos, txt.size(), skipEmpty, doTrim);

    return result;
  }

  template<typename T> requires IsCharacter<T>
  inline StringList<T> split(const String<T>& txt, const T *del,
                             const bool skipEmpty = false, const bool doTrim = false)
  {
    return split<T>(txt, del, MAX_SIZE_T, skipEmpty, doTrim);
  }

  template<typename T> requires IsCharacter<T>
  inline StringList<T> split(const String<T>& txt, const String<T>& del,
                             const bool skipEmpty = false, const bool doTrim = false)
  {
    return split<T>(txt, del.data(), del.size(), skipEmpty, doTrim);
  }

  template<typename T> requires IsCharacter<T>
  inline StringList<T> split(const String<T>& txt, const T& del,
                             const bool skipEmpty = false, const bool doTrim = false)
  {
    return split<T>(txt, &del, 1, skipEmpty, doTrim);
  }

  ////// String starts with pattern... ///////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  inline bool startsWith(const T *str, const std::size_t lenstr, // haystack
                         const T *pat, const std::size_t lenpat, // needle
                         const bool ignoreCase = false) noexcept
  {
    const std::size_t maxstr = lenstr == MAX_SIZE_T
        ? length(str)
        : lenstr;
    const std::size_t maxpat = lenpat == MAX_SIZE_T
        ? length(pat)
        : lenpat;

    if( str == nullptr  ||  pat == nullptr  ||
        maxstr < 1  ||  maxpat < 1  ||  maxstr < maxpat ) {
      return false;
    }

    return ignoreCase
        ? std::equal(str, str + maxpat, pat, lambda_eqI<T>())
        : std::equal(str, str + maxpat, pat);
  }

  template<typename T> requires IsCharacter<T>
  inline bool startsWith(const T *str, const T *pat, const bool ignoreCase = false)
  {
    return startsWith<T>(str, MAX_SIZE_T, pat, MAX_SIZE_T, ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool startsWith(const String<T>& str, const T *pat, const bool ignoreCase = false)
  {
    return startsWith<T>(str.data(), str.size(), pat, MAX_SIZE_T, ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool startsWith(const T *str, const String<T>& pat, const bool ignoreCase = false)
  {
    return startsWith<T>(str, MAX_SIZE_T, pat.data(), pat.size(), ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool startsWith(const String<T>& str, const String<T>& pat, const bool ignoreCase = false)
  {
    return startsWith<T>(str.data(), str.size(), pat.data(), pat.size(), ignoreCase);
  }

  ////// Case conversion... //////////////////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  inline void toLower(T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = len == MAX_SIZE_T
        ? length(str)
        : len;

    if( str == nullptr  ||
        max < 1 ) {
      return;
    }

    std::for_each(str, str + max, lambda_to_lower<T>());
  }

  template<typename T> requires IsCharacter<T>
  inline void toLower(String<T> *str)
  {
    toLower<T>(str->data(), str->size());
  }

  template<typename T> requires IsCharacter<T>
  inline void toUpper(T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = len == MAX_SIZE_T
        ? length(str)
        : len;

    if( str == nullptr  ||
        max < 1 ) {
      return;
    }

    std::for_each(str, str + max, lambda_to_upper<T>());
  }

  template<typename T> requires IsCharacter<T>
  inline void toUpper(String<T> *str)
  {
    toUpper<T>(str->data(), str->size());
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
