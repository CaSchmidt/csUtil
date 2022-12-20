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

#include <cs/Core/StringRange.h>

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

  template<typename T>
  requires IsCharacter<T>
  inline std::size_t lengthRange(const String<T>& str)
  {
    return lengthRange(str.data(), str.size());
  }

  ////// String ends with pattern... /////////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline bool endsWith(const T *str, const std::size_t lenstr,
                       const T *pat, const std::size_t lenpat,
                       const bool ignoreCase = false)
  {
    const std::size_t maxstr = length(str, lenstr);
    const std::size_t maxpat = length(pat, lenpat);

    if( !isValid(str, maxstr)  ||  !isValid(pat, maxpat)  ||
        maxstr < maxpat ) {
      return false;
    }

    return ignoreCase
        ? std::equal(str + maxstr - maxpat, str + maxstr, pat, lambda_eqI<T>())
        : std::equal(str + maxstr - maxpat, str + maxstr, pat);
  }

  template<typename T> requires IsCharacter<T>
  inline bool endsWith(const T *str, const T *pat, const bool ignoreCase = false)
  {
    return endsWith(str, MAX_SIZE_T, pat, MAX_SIZE_T, ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool endsWith(const String<T>& str, const T *pat, const bool ignoreCase = false)
  {
    return endsWith(str.data(), str.size(), pat, MAX_SIZE_T, ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool endsWith(const T *str, const String<T>& pat, const bool ignoreCase = false)
  {
    return endsWith(str, MAX_SIZE_T, pat.data(), pat.size(), ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool endsWith(const String<T>& str, const String<T>& pat, const bool ignoreCase = false)
  {
    return endsWith(str.data(), str.size(), pat.data(), pat.size(), ignoreCase);
  }

  ////// Strings are equal in the first N characters... //////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline bool equalsN(const T *a, const T *b, const std::size_t len,
                      const bool ignoreCase = false)
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
    return equalsN(a, b, MAX_SIZE_T, ignoreCase);
  }

  ////// Strings are equal... ////////////////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline bool equals(const T *a, const std::size_t lena,
                     const T *b, const std::size_t lenb,
                     const bool ignoreCase = false)
  {
    const std::size_t maxa = length(a, lena);
    const std::size_t maxb = length(b, lenb);

    if( !isValid(a, maxa)  ||  !isValid(b, maxb)  ||
        maxa != maxb ) {
      return false;
    }

    return ignoreCase
        ? std::equal(a, a + maxa, b, lambda_eqI<T>())
        : std::equal(a, a + maxa, b);
  }

  template<typename T> requires IsCharacter<T>
  inline bool equals(const T *a, const T *b, const bool ignoreCase = false)
  {
    return equals(a, MAX_SIZE_T, b, MAX_SIZE_T, ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool equals(const String<T>& a, const T *b, const bool ignoreCase = false)
  {
    return equals(a.data(), a.size(), b, MAX_SIZE_T, ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool equals(const T *a, const String<T>& b, const bool ignoreCase = false)
  {
    return equals(a, MAX_SIZE_T, b.data(), b.size(), ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool equals(const String<T>& a, const String<T>& b, const bool ignoreCase = false)
  {
    return equals(a.data(), a.size(), b.data(), b.size(), ignoreCase);
  }

  ////// String is C-style identifier... /////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline bool isIdent(const T *first, const T *last)
  {
    if( !isValid(first, last) ) {
      return false;
    }

    return isIdentFirst(*first)  &&
        std::all_of(first + 1, last, lambda_is_ident<T>());
  }

  template<typename T> requires IsCharacter<T>
  inline bool isIdent(const T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = length(str, len);
    return isIdent(str, str + max);
  }

  template<typename T> requires IsCharacter<T>
  inline bool isIdent(const String<T>& str)
  {
    return isIdent(str.data(), str.size());
  }

  ////// String contains only whitespace... //////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline bool isSpace(const T *first, const T *last)
  {
    if( !isValid(first, last) ) {
      return false;
    }

    return std::all_of(first, last, lambda_is_space<T>());
  }

  template<typename T> requires IsCharacter<T>
  inline bool isSpace(const T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = length(str, len);
    return isSpace(str, str + max);
  }

  template<typename T> requires IsCharacter<T>
  inline bool isSpace(const String<T>& str)
  {
    return isSpace(str.data(), str.size());
  }

  ////// Remove pattern from string... ///////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline void removeAll(T *first, T* last,
                        const T *pat, const std::size_t lenpat = MAX_SIZE_T)
  {
    const std::size_t maxpat = length(pat, lenpat);

    if( !isValid(first, last)  ||  !isValid(pat, maxpat)  ||
        distance(first, last) < maxpat ) {
      return;
    }

    T *end = last;
    for(T *hit = first; (hit = std::search(hit, end, pat, pat + maxpat)) != end; ) {
      end = std::copy(hit + maxpat, end, hit);
    }
    std::for_each(end, last, lambda_set_null<T>());
  }

  template<typename T> requires IsCharacter<T>
  inline void removeAll(T *str, const std::size_t lenstr,
                        const T *pat, const std::size_t lenpat = MAX_SIZE_T)
  {
    const std::size_t maxstr = length(str, lenstr);
    removeAll(str, str + maxstr, pat, lenpat);
  }

  template<typename T> requires IsCharacter<T>
  inline void removeAll(T *str,
                        const T *pat, const std::size_t lenpat = MAX_SIZE_T)
  {
    removeAll(str, MAX_SIZE_T, pat, lenpat);
  }

  template<typename T> requires IsCharacter<T>
  inline void removeAll(T *str, const std::size_t lenstr,
                        const String<T>& pat)
  {
    removeAll(str, lenstr, pat.data(), pat.size());
  }

  template<typename T> requires IsCharacter<T>
  inline void removeAll(T *str,
                        const String<T>& pat)
  {
    removeAll(str, MAX_SIZE_T, pat);
  }

  template<typename T> requires IsCharacter<T>
  inline void removeAll(String<T> *str,
                        const T *pat, const std::size_t lenpat = MAX_SIZE_T)
  {
    removeAll(str->data(), str->size(), pat, lenpat);
    str->resize(lengthRange(*str));
  }

  template<typename T> requires IsCharacter<T>
  inline void removeAll(String<T> *str,
                        const String<T>& pat)
  {
    removeAll(str, pat.data(), pat.size());
  }

  ////// Remove character from string... /////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline void removeAll(T *first, T *last, const T& pat)
  {
    if( !isValid(first, last) ) {
      return;
    }

    T *end = std::remove(first, last, pat);
    std::for_each(end, last, lambda_set_null<T>());
  }

  template<typename T> requires IsCharacter<T>
  inline void removeAll(T *str, const std::size_t len, const T& pat)
  {
    const std::size_t max = length(str, len);
    removeAll(str, str + max, pat);
  }

  template<typename T> requires IsCharacter<T>
  inline void removeAll(T *str, const T& pat)
  {
    removeAll(str, MAX_SIZE_T, pat);
  }

  template<typename T> requires IsCharacter<T>
  inline void removeAll(String<T> *str, const T& pat)
  {
    removeAll(str->data(), str->size(), pat);
    str->resize(lengthRange(*str));
  }

  ////// Remove character matching predicate from string... //////////////////

  template<typename T, typename PredFunc>
  requires IsCharacter<T>
  inline void removeAll(T *first, T *last, PredFunc func)
  {
    if( !isValid(first, last) ) {
      return;
    }

    T *end = std::remove_if(first, last, func);
    std::for_each(end, last, lambda_set_null<T>());
  }

  template<typename T, typename PredFunc> requires IsCharacter<T>
  inline void removeAll(T *str, const std::size_t len, PredFunc func)
  {
    const std::size_t max = length(str, len);
    removeAll<T,PredFunc>(str, str + max, func);
  }

  template<typename T, typename PredFunc> requires IsCharacter<T>
  inline void removeAll(T *str, PredFunc func)
  {
    removeAll<T,PredFunc>(str, MAX_SIZE_T, func);
  }

  template<typename T, typename PredFunc> requires IsCharacter<T>
  inline void removeAll(String<T> *str, PredFunc func)
  {
    removeAll<T,PredFunc>(str->data(), str->size(), func);
    str->resize(lengthRange(*str));
  }

  ////// Remove Trailing Zeros from Fixed-Notation Floating-Point String /////

  template<typename T>
  requires IsCharacter<T>
  inline void removeTrailingZeros(T *first, T *last,
                                  const bool removeDot = true)
  {
    using RevIter = std::reverse_iterator<T*>;

    using g = glyph<T>;

    // (0) Sanity Check //////////////////////////////////////////////////////

    if( !isValid(first, last) ) {
      return;
    }

    // (1) Find '.' //////////////////////////////////////////////////////////

    T *dot = std::find(first, last, g::dot);
    if( dot == last ) { // Nothing to do!
      return;
    }

    // (2) Do not touch exponent notation! ///////////////////////////////////

    if( std::find(dot + 1, last, g::e) != last ) {
      return;
    }

    if( std::find(dot + 1, last, g::E) != last ) {
      return;
    }

    // (3) Remove Trailing Zeros /////////////////////////////////////////////

    T *end = std::find_if_not(RevIter{last}, RevIter{dot},
                              lambda_is_zero<T>()).base();
    std::for_each(end, last, lambda_set_null<T>());

    if( removeDot  &&  end == dot + 1 ) {
      *dot = g::null;
    }
  }

  template<typename T> requires IsCharacter<T>
  inline void removeTrailingZeros(T *str, const std::size_t len,
                                  const bool removeDot = true)
  {
    const std::size_t max = lengthMax(str, len);
    removeTrailingZeros(str, str + max, removeDot);
  }

  template<typename T> requires IsCharacter<T>
  inline void removeTrailingZeros(T *str,
                                  const bool removeDot = true)
  {
    removeTrailingZeros(str, MAX_SIZE_T, removeDot);
  }

  template<typename T> requires IsCharacter<T>
  inline void removeTrailingZeros(String<T> *str, const bool removeDot = true)
  {
    removeTrailingZeros(str->data(), str->size(), removeDot);
    str->resize(lengthRange(*str));
  }

  ////// Replace pattern in string... ////////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline void replaceAll(String<T> *str,
                         const T *pat, const std::size_t lenpat,
                         const T *txt, const std::size_t lentxt = MAX_SIZE_T)
  {
    const std::size_t maxpat = length(pat, lenpat);
    const std::size_t maxtxt = length(txt, lentxt);

    if( str->size() < 1  ||  !isValid(pat, maxpat)  ||  !isValid(txt, maxtxt)  ||
        str->size() < maxpat ) {
      return;
    }

    for(std::size_t pos = 0; (pos = str->find(pat, pos, maxpat)) != NPOS<T>; pos += maxtxt) {
      str->replace(pos, maxpat, txt, maxtxt);
    }
  }

  template<typename T> requires IsCharacter<T>
  inline void replaceAll(String<T> *str, const T *pat, const T *txt)
  {
    replaceAll(str, pat, MAX_SIZE_T, txt, MAX_SIZE_T);
  }

  template<typename T> requires IsCharacter<T>
  inline void replaceAll(String<T> *str, const String<T>& pat, const T *txt)
  {
    replaceAll(str, pat.data(), pat.size(), txt, MAX_SIZE_T);
  }

  template<typename T> requires IsCharacter<T>
  inline void replaceAll(String<T> *str, const T *pat, const String<T>& txt)
  {
    replaceAll(str, pat, MAX_SIZE_T, txt.data(), txt.size());
  }

  template<typename T> requires IsCharacter<T>
  inline void replaceAll(String<T> *str, const String<T>& pat, const String<T>& txt)
  {
    replaceAll(str, pat.data(), pat.size(), txt.data(), txt.size());
  }

  template<typename T> requires IsCharacter<T>
  inline void replaceAll(String<T> *str, const T& pat, const char *txt)
  {
    replaceAll(str, &pat, 1, txt, MAX_SIZE_T);
  }

  template<typename T> requires IsCharacter<T>
  inline void replaceAll(String<T> *str, const T& pat, const String<T>& txt)
  {
    replaceAll(str, &pat, 1, txt.data(), txt.size());
  }

  ////// Reclaim memory... ///////////////////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline void shrink(String<T> *str)
  {
    str->resize(lengthRange(*str));
    str->shrink_to_fit();
  }

  ////// Remove whitespace from begin & end... ///////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline void trim(T *first, T *last)
  {
    using RevIter = std::reverse_iterator<T*>;

    if( !isValid(first, last) ) {
      return;
    }

    // trim left
    T *lend = std::copy(std::find_if_not(first, last,
                                         lambda_is_space<T>()),
                        last,
                        first);
    // trim right
    T *rend = std::find_if_not(RevIter{lend}, RevIter{first},
                               lambda_is_space<T>()).base();
    // fill with NULL
    std::for_each(rend, last, lambda_set_null<T>());
  }

  template<typename T> requires IsCharacter<T>
  inline void trim(T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = lengthMax(str, len);
    trim(str, str + max);
  }

  template<typename T> requires IsCharacter<T>
  inline void trim(String<T> *str)
  {
    trim(str->data(), str->size());
    str->resize(lengthRange(*str));
  }

  template<typename T> requires IsCharacter<T>
  inline String<T> trimmed(String<T> str)
  {
    trim(&str);
    return str;
  }

  ////// Replace consecutive whitespace with single space... /////////////////

  template<typename T> requires IsCharacter<T>
  inline void simplify(T *first, T *last)
  {
    constexpr auto lambda_adjacent_space = [](const T& a, const T& b) -> bool {
      return isSpace(a)  &&  isSpace(b);
    };

    if( !isValid(first, last) ) {
      return;
    }

    // (1) remove duplicate whitespace
    T *end = std::unique(first, last, lambda_adjacent_space);
    // (1.5) fill with NULL
    std::for_each(end, last, lambda_set_null<T>());
    // (2) replace single whitespace characters with space
    std::replace_if(first, end, lambda_is_space<T>(), glyph<T>::space);
    // (3) trim result
    trim(first, end);
  }

  template<typename T> requires IsCharacter<T>
  inline void simplify(T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = lengthMax(str, len);
    simplify(str, str + max);
  }

  template<typename T> requires IsCharacter<T>
  inline void simplify(String<T> *str)
  {
    simplify(str->data(), str->size());
    str->resize(lengthRange(*str));
  }

  template<typename T> requires IsCharacter<T>
  inline String<T> simplified(String<T> str)
  {
    simplify(&str);
    return str;
  }

  ////// Split string at pattern... //////////////////////////////////////////

  namespace impl_string {

    template<typename T>
    requires IsCharacter<T>
    inline void extract(StringList<T> *result,
                        const T *first, const T *last,
                        const bool skipEmpty, const bool doTrim)
    {
      const std::size_t len = distance(first, last);
      String<T> part = len > 0
          ? String<T>{first, len}
          : String<T>{};

      if( skipEmpty  &&  part.empty() ) {
        return;
      }

      if( doTrim ) {
        trim(&part);
      }

      result->push_back(std::move(part));
    }

  } // namespace impl_string

  template<typename T>
  requires IsCharacter<T>
  inline StringList<T> split(const T *first, const T *last,
                             const T *pat, const std::size_t lenpat = MAX_SIZE_T,
                             const bool skipEmpty = false, const bool doTrim = false)
  {
    const std::size_t maxpat = length(pat, lenpat);

    if( !isValid(first, last)  ||  !isValid(pat, maxpat)  ||
        distance(first, last) < maxpat ) {
      return StringList<T>{};
    }

    StringList<T> result;

    const T *from = first;
    for(const T *hit;
        (hit = std::search(from, last, pat, pat + maxpat)) != last;
        from = hit + maxpat) {
      impl_string::extract(&result, from, hit, skipEmpty, doTrim);
    }
    impl_string::extract(&result, from, last, skipEmpty, doTrim);

    return result;
  }

  template<typename T> requires IsCharacter<T>
  inline StringList<T> split(const T *str, const std::size_t lenstr,
                             const T *pat, const std::size_t lenpat,
                             const bool skipEmpty = false, const bool doTrim = false)
  {
    const std::size_t maxstr = length(str, lenstr);
    return split(str, str + maxstr, pat, lenpat, skipEmpty, doTrim);
  }

  template<typename T> requires IsCharacter<T>
  inline StringList<T> split(const T *str,
                             const T *pat,
                             const bool skipEmpty = false, const bool doTrim = false)
  {
    return split(str, MAX_SIZE_T, pat, MAX_SIZE_T, skipEmpty, doTrim);
  }

  template<typename T> requires IsCharacter<T>
  inline StringList<T> split(const String<T>& str,
                             const T *pat,
                             const bool skipEmpty = false, const bool doTrim = false)
  {
    return split(str.data(), str.size(), pat, MAX_SIZE_T, skipEmpty, doTrim);
  }

  ////// Split string at character... ////////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline StringList<T> split(const T *first, const T *last,
                             const T& pat,
                             const bool skipEmpty = false, const bool doTrim = false)
  {
    if( !isValid(first, last) ) {
      return StringList<T>{};
    }

    StringList<T> result;

    const T *from = first;
    for(const T *hit;
        (hit = std::find(from, last, pat)) != last;
        from = hit + 1) {
      impl_string::extract(&result, from, hit, skipEmpty, doTrim);
    }
    impl_string::extract(&result, from, last, skipEmpty, doTrim);

    return result;
  }

  template<typename T> requires IsCharacter<T>
  inline StringList<T> split(const T *str, const std::size_t len,
                             const T& pat,
                             const bool skipEmpty = false, const bool doTrim = false)
  {
    const std::size_t max = len == MAX_SIZE_T
        ? length(str)
        : len;
    return split(str, str + max, pat, skipEmpty, doTrim);
  }

  template<typename T> requires IsCharacter<T>
  inline StringList<T> split(const T *str,
                             const T& pat,
                             const bool skipEmpty = false, const bool doTrim = false)
  {
    return split(str, MAX_SIZE_T, pat, skipEmpty, doTrim);
  }

  template<typename T> requires IsCharacter<T>
  inline StringList<T> split(const String<T>& str,
                             const T& pat,
                             const bool skipEmpty = false, const bool doTrim = false)
  {
    return split(str.data(), str.size(), pat, skipEmpty, doTrim);
  }

  ////// String starts with pattern... ///////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline bool startsWith(const T *str, const std::size_t lenstr, // haystack
                         const T *pat, const std::size_t lenpat, // needle
                         const bool ignoreCase = false)
  {
    const std::size_t maxstr = length(str, lenstr);
    const std::size_t maxpat = length(pat, lenpat);

    if( !isValid(str, maxstr)  ||  !isValid(pat, maxpat)  ||
        maxstr < maxpat ) {
      return false;
    }

    return ignoreCase
        ? std::equal(str, str + maxpat, pat, lambda_eqI<T>())
        : std::equal(str, str + maxpat, pat);
  }

  template<typename T> requires IsCharacter<T>
  inline bool startsWith(const T *str, const T *pat, const bool ignoreCase = false)
  {
    return startsWith(str, MAX_SIZE_T, pat, MAX_SIZE_T, ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool startsWith(const String<T>& str, const T *pat, const bool ignoreCase = false)
  {
    return startsWith(str.data(), str.size(), pat, MAX_SIZE_T, ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool startsWith(const T *str, const String<T>& pat, const bool ignoreCase = false)
  {
    return startsWith(str, MAX_SIZE_T, pat.data(), pat.size(), ignoreCase);
  }

  template<typename T> requires IsCharacter<T>
  inline bool startsWith(const String<T>& str, const String<T>& pat, const bool ignoreCase = false)
  {
    return startsWith(str.data(), str.size(), pat.data(), pat.size(), ignoreCase);
  }

  ////// Case conversion... //////////////////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline void toLower(T *first, T *last)
  {
    if( !isValid(first, last) ) {
      return;
    }

    std::for_each(first, last, lambda_to_lower<T>());
  }

  template<typename T> requires IsCharacter<T>
  inline void toLower(T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = length(str, len);
    toLower(str, str + max);
  }

  template<typename T> requires IsCharacter<T>
  inline void toLower(String<T> *str)
  {
    toLower(str->data(), str->size());
  }

  template<typename T>
  requires IsCharacter<T>
  inline void toUpper(T *first, T *last)
  {
    if( !isValid(first, last) ) {
      return;
    }

    std::for_each(first, last, lambda_to_upper<T>());
  }

  template<typename T> requires IsCharacter<T>
  inline void toUpper(T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = length(str, len);
    toUpper(str, str + max);
  }

  template<typename T> requires IsCharacter<T>
  inline void toUpper(String<T> *str)
  {
    toUpper(str->data(), str->size());
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
