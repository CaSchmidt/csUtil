/****************************************************************************
** Copyright (c) 2022, Carsten Schmidt. All rights reserved.
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

#include <cs/Core/StringRange.h>

namespace cs {

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

  template<typename T>
  requires IsCharacter<T>
  inline bool endsWith(const T *str, const T *pat, const bool ignoreCase = false)
  {
    return endsWith(str, MAX_SIZE_T, pat, MAX_SIZE_T, ignoreCase);
  }

  ////// Strings are equal in the first N characters... //////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline bool equalsN(const T *a, const T *b, const std::size_t len,
                      const bool ignoreCase = false)
  {
    const std::size_t max = std::min(length(a, len), length(b, len));

    if( max < 1 ) {
      return false;
    }

    return ignoreCase
        ? std::equal(a, a + max, b, lambda_eqI<T>())
        : std::equal(a, a + max, b);
  }

  template<typename T>
  requires IsCharacter<T>
  inline bool equalsN(const T *a, const T *b, const bool ignoreCase = false)
  {
    return equalsN(a, b, MAX_SIZE_T, ignoreCase);
  }

  ////// String are equal... /////////////////////////////////////////////////

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

  template<typename T>
  requires IsCharacter<T>
  inline bool equals(const T *a, const T *b, const bool ignoreCase = false)
  {
    return equals(a, MAX_SIZE_T, b, MAX_SIZE_T, ignoreCase);
  }

  ////// String is hexadecimal string... /////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  inline bool isHexString(const T *first, const T *last)
  {
    if( !isValid(first, last) ) {
      return false;
    }

    return std::all_of(first, last, lambda_is_hex<T>());
  }

  template<typename T> requires IsCharacter<T>
  inline bool isHexString(const T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = length(str, len);
    return isHexString(str, str + max);
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

  template<typename T>
  requires IsCharacter<T>
  inline bool isIdent(const T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = length(str, len);
    return isIdent(str, str + max);
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

  template<typename T>
  requires IsCharacter<T>
  inline bool isSpace(const T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = length(str, len);
    return isSpace(str, str + max);
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
    for(T *hit = first;
        (hit = std::search(hit, end, pat, pat + maxpat)) != end;
        ) {
      end = std::copy(hit + maxpat, end, hit);
    }
    std::for_each(end, last, lambda_set_null<T>());
  }

  template<typename T>
  requires IsCharacter<T>
  inline void removeAll(T *str, const std::size_t lenstr,
                        const T *pat, const std::size_t lenpat = MAX_SIZE_T)
  {
    const std::size_t maxstr = length(str, lenstr);
    removeAll(str, str + maxstr, pat, lenpat);
  }

  template<typename T>
  requires IsCharacter<T>
  inline void removeAll(T *str,
                        const T *pat, const std::size_t lenpat = MAX_SIZE_T)
  {
    removeAll(str, MAX_SIZE_T, pat, lenpat);
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

  template<typename T>
  requires IsCharacter<T>
  inline void removeAll(T *str, const std::size_t len, const T& pat)
  {
    const std::size_t max = length(str, len);
    removeAll(str, str + max, pat);
  }

  template<typename T>
  requires IsCharacter<T>
  inline void removeAll(T *str, const T& pat)
  {
    removeAll(str, MAX_SIZE_T, pat);
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

  template<typename T, typename PredFunc>
  requires IsCharacter<T>
  inline void removeAll(T *str, const std::size_t len, PredFunc func)
  {
    const std::size_t max = length(str, len);
    removeAll<T,PredFunc>(str, str + max, func);
  }

  template<typename T, typename PredFunc>
  requires IsCharacter<T>
  inline void removeAll(T *str, PredFunc func)
  {
    removeAll<T,PredFunc>(str, MAX_SIZE_T, func);
  }

  ////// Remove trailing zeros from Fixed-Notation Floating-Point string... //

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

  template<typename T>
  requires IsCharacter<T>
  inline void removeTrailingZeros(T *str, const std::size_t len,
                                  const bool removeDot = true)
  {
    const std::size_t max = lengthMax(str, len);
    removeTrailingZeros(str, str + max, removeDot);
  }

  template<typename T>
  requires IsCharacter<T>
  inline void removeTrailingZeros(T *str,
                                  const bool removeDot = true)
  {
    removeTrailingZeros(str, MAX_SIZE_T, removeDot);
  }

  ////// Replace character in string... //////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline void replaceAll(T *first, T *last, const T& pat, const T& txt)
  {
    if( !isValid(first, last) ) {
      return;
    }

    std::replace(first, last, pat, txt);
  }

  template<typename T>
  requires IsCharacter<T>
  inline void replaceAll(T *str, const std::size_t len,
                         const T& pat, const T& txt)
  {
    const std::size_t max = length(str, len);
    replaceAll(str, str + max, pat, txt);
  }

  template<typename T>
  requires IsCharacter<T>
  inline void replaceAll(T *str,
                         const T& pat, const T& txt)
  {
    replaceAll(str, MAX_SIZE_T, pat, txt);
  }

  ////// Replace character matching predicate in string... ///////////////////

  template<typename T, typename PredFunc>
  requires IsCharacter<T>
  void replaceAll(T *first, T *last, PredFunc func, const T& txt)
  {
    if( !isValid(first, last) ) {
      return;
    }

    std::replace_if(first, last, func, txt);
  }

  template<typename T, typename PredFunc>
  requires IsCharacter<T>
  void replaceAll(T *str, const std::size_t len,
                  PredFunc func, const T& txt)
  {
    const std::size_t max = length(str, len);
    replaceAll<T,PredFunc>(str, str + max, func, txt);
  }

  template<typename T, typename PredFunc>
  requires IsCharacter<T>
  void replaceAll(T *str,
                  PredFunc func, const T& txt)
  {
    replaceAll<T,PredFunc>(str, MAX_SIZE_T, func, txt);
  }

  ////// Remove whitespace from begin & end of string... /////////////////////

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

  template<typename T>
  requires IsCharacter<T>
  inline void trim(T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = lengthMax(str, len);
    trim(str, str + max);
  }

  ////// Replace consecutive whitespace with single space... /////////////////

  template<typename T>
  requires IsCharacter<T>
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

  template<typename T>
  requires IsCharacter<T>
  inline void simplify(T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = lengthMax(str, len);
    simplify(str, str + max);
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

  template<typename T>
  requires IsCharacter<T>
  inline bool startsWith(const T *str, const T *pat, const bool ignoreCase = false)
  {
    return startsWith(str, MAX_SIZE_T, pat, MAX_SIZE_T, ignoreCase);
  }

  ////// Convert to lower case... ////////////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline void toLower(T *first, T *last)
  {
    if( !isValid(first, last) ) {
      return;
    }

    std::for_each(first, last, lambda_to_lower<T>());
  }

  template<typename T>
  requires IsCharacter<T>
  inline void toLower(T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = length(str, len);
    toLower(str, str + max);
  }

  ////// Convert to upper case... ////////////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  inline void toUpper(T *first, T *last)
  {
    if( !isValid(first, last) ) {
      return;
    }

    std::for_each(first, last, lambda_to_upper<T>());
  }

  template<typename T>
  requires IsCharacter<T>
  inline void toUpper(T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = length(str, len);
    toUpper(str, str + max);
  }

  ////// Widen string... /////////////////////////////////////////////////////

  template<typename WCharT, typename NCharT>
  requires IsWideCharacter<WCharT>  &&  IsNarrowCharacter<NCharT>
  inline void widen(WCharT *dest, const NCharT *first, const NCharT *last)
  {
    constexpr auto lambda_widen = [](const NCharT& c) -> WCharT {
      return static_cast<WCharT>(c);
    };

    if( dest == nullptr  ||  !isValid(first, last) ) {
      return;
    }

    std::transform(first, last, dest, lambda_widen);
  }

  template<typename WCharT, typename NCharT>
  requires IsWideCharacter<WCharT>  &&  IsNarrowCharacter<NCharT>
  inline void widen(WCharT *dest, const NCharT *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = length(str, len);
    widen(dest, str, str + max);
  }

} // namespace cs
