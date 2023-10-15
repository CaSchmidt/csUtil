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

#include <cs/Core/Range.h>

namespace cs {

  ////// String contains character... ////////////////////////////////////////

  namespace impl_string {

    template<typename T>
    requires is_char_v<T>
    inline bool contains(const T *first, const T *last, const T& pat)
    {
      return std::find(first, last, pat) != last;
    }

  } // namespace impl_string

  template<typename T>
  requires is_char_v<T>
  inline bool contains(const T *first, const T *last, const T& pat)
  {
    return Pointer::isValidRange(first, last)
        ? impl_string::contains(first, last, pat)
        : false;
  }

  template<typename T>
  requires is_char_v<T>
  inline bool contains(const T *str, const std::size_t len, const T& pat)
  {
    const std::size_t max = strlen(str, len);
    return max > 0
        ? impl_string::contains(str, str + max, pat)
        : false;
  }

  template<typename T>
  requires is_char_v<T>
  inline bool contains(const T *str, const T& pat)
  {
    return contains(str, MAX_SIZE_T, pat);
  }

  ////// String contains predicate... ////////////////////////////////////////

  namespace impl_string {

    template<typename T, typename PredFunc>
    requires is_char_v<T>
    inline bool contains(const T *first, const T *last, PredFunc func)
    {
      return std::find_if(first, last, func) != last;
    }

  } // namespace impl_string

  template<typename T, typename PredFunc>
  requires is_char_v<T>
  inline bool contains(const T *first, const T *last, PredFunc func)
  {
    return Pointer::isValidRange(first, last)
        ? impl_string::contains<T,PredFunc>(first, last, func)
        : false;
  }

  template<typename T, typename PredFunc>
  requires is_char_v<T>
  inline bool contains(const T *str, const std::size_t len, PredFunc func)
  {
    const std::size_t max = strlen(str, len);
    return max > 0
        ? impl_string::contains<T,PredFunc>(str, str + max, func)
        : false;
  }

  template<typename T, typename PredFunc>
  requires is_char_v<T>
  inline bool contains(const T *str, PredFunc func)
  {
    return contains<T,PredFunc>(str, MAX_SIZE_T, func);
  }

  ////// String contains pattern... //////////////////////////////////////////

  namespace impl_string {

    template<typename T>
    requires is_char_v<T>
    inline bool contains(const T *strfirst, const T *strlast,
                         const T *patfirst, const T *patlast,
                         const bool ignoreCase)
    {
      const T *hit = ignoreCase
          ? std::search(strfirst, strlast, patfirst, patlast, lambda_eqI<T>())
          : std::search(strfirst, strlast, patfirst, patlast);
      return hit != strlast;
    }

  } // namespace impl_string

  template<typename T>
  requires is_char_v<T>
  inline bool contains(const T *strfirst, const T *strlast,
                       const T *patfirst, const T *patlast,
                       const bool ignoreCase = false)
  {
    const bool str_valid = Pointer::isValidRange(strfirst, strlast);
    const bool pat_valid = Pointer::isValidRange(patfirst, patlast);
    return str_valid  &&  pat_valid
        ? impl_string::contains(strfirst, strlast, patfirst, patlast, ignoreCase)
        : false;
  }

  template<typename T>
  requires is_char_v<T>
  inline bool contains(const T *str, const std::size_t lenstr,
                       const T *pat, const std::size_t lenpat,
                       const bool ignoreCase = false)
  {
    const std::size_t maxstr = strlen(str, lenstr);
    const std::size_t maxpat = strlen(pat, lenpat);
    return maxstr > 0  &&  maxpat
        ? impl_string::contains(str, str + maxstr, pat, pat + maxpat, ignoreCase)
        : false;
  }

  template<typename T>
  requires is_char_v<T>
  inline bool contains(const T *str,
                       const T *pat,
                       const bool ignoreCase = false)
  {
    return contains(str, MAX_SIZE_T, pat, MAX_SIZE_T, ignoreCase);
  }

  ////// String ends with pattern... /////////////////////////////////////////

  template<typename T>
  requires is_char_v<T>
  inline bool endsWith(const T *str, const std::size_t lenstr,
                       const T *pat, const std::size_t lenpat,
                       const bool ignoreCase = false)
  {
    const std::size_t maxstr = strlen(str, lenstr);
    const std::size_t maxpat = strlen(pat, lenpat);

    if( maxstr > 0  &&  maxpat > 0  &&  maxstr >= maxpat ) {
      return ignoreCase
          ? std::equal(str + maxstr - maxpat, str + maxstr, pat, lambda_eqI<T>())
          : std::equal(str + maxstr - maxpat, str + maxstr, pat);
    }

    return false;
  }

  template<typename T>
  requires is_char_v<T>
  inline bool endsWith(const T *str, const T *pat, const bool ignoreCase = false)
  {
    return endsWith(str, MAX_SIZE_T, pat, MAX_SIZE_T, ignoreCase);
  }

  ////// Strings are equal in the first N characters... //////////////////////

  template<typename T>
  requires is_char_v<T>
  inline bool equalsN(const T *a, const T *b, const std::size_t len,
                      const bool ignoreCase = false)
  {
    const std::size_t max = std::min(strlen(a, len), strlen(b, len));

    if( max > 0 ) {
      return ignoreCase
          ? std::equal(a, a + max, b, lambda_eqI<T>())
          : std::equal(a, a + max, b);
    }

    return false;
  }

  template<typename T>
  requires is_char_v<T>
  inline bool equalsN(const T *a, const T *b, const bool ignoreCase = false)
  {
    return equalsN(a, b, MAX_SIZE_T, ignoreCase);
  }

  ////// String are equal... /////////////////////////////////////////////////

  template<typename T>
  requires is_char_v<T>
  inline bool equals(const T *a, const std::size_t lena,
                     const T *b, const std::size_t lenb,
                     const bool ignoreCase = false)
  {
    const std::size_t maxa = strlen(a, lena);
    const std::size_t maxb = strlen(b, lenb);

    if( maxa > 0  &&  maxb > 0  &&  maxa == maxb ) {
      return ignoreCase
          ? std::equal(a, a + maxa, b, lambda_eqI<T>())
          : std::equal(a, a + maxa, b);
    }

    return false;
  }

  template<typename T>
  requires is_char_v<T>
  inline bool equals(const T *a, const T *b, const bool ignoreCase = false)
  {
    return equals(a, MAX_SIZE_T, b, MAX_SIZE_T, ignoreCase);
  }

  ////// String is hexadecimal string... /////////////////////////////////////

  namespace impl_string {

    template<typename T>
    requires is_char_v<T>
    inline bool isHexString(const T *first, const T *last)
    {
      return std::all_of(first, last, lambda_is_hex<T>());
    }

  } // namespace impl_string

  template<typename T>
  requires is_char_v<T>
  inline bool isHexString(const T *first, const T *last)
  {
    return Pointer::isValidRange(first, last)
        ? impl_string::isHexString(first, last)
        : false;
  }

  template<typename T>
  requires is_char_v<T>
  inline bool isHexString(const T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = strlen(str, len);
    return max > 0
        ? impl_string::isHexString(str, str + max)
        : false;
  }

  ////// String is C-style identifier... /////////////////////////////////////

  namespace impl_string {

    template<typename T>
    requires is_char_v<T>
    inline bool isIdent(const T *first, const T *last)
    {
      return isIdentFirst(*first)  &&
          std::all_of(first + 1, last, lambda_is_ident<T>());
    }

  } // namespace impl_string

  template<typename T>
  requires is_char_v<T>
  inline bool isIdent(const T *first, const T *last)
  {
    return Pointer::isValidRange(first, last)
        ? impl_string::isIdent(first, last)
        : false;
  }

  template<typename T>
  requires is_char_v<T>
  inline bool isIdent(const T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = strlen(str, len);
    return max > 0
        ? impl_string::isIdent(str, str + max)
        : false;
  }

  ////// String contains only whitespace... //////////////////////////////////

  namespace impl_string {

    template<typename T>
    requires is_char_v<T>
    inline bool isSpace(const T *first, const T *last)
    {
      return std::all_of(first, last, lambda_is_space<T>());
    }

  } // namespace impl_string

  template<typename T>
  requires is_char_v<T>
  inline bool isSpace(const T *first, const T *last)
  {
    return Pointer::isValidRange(first, last)
        ? impl_string::isSpace(first, last)
        : false;
  }

  template<typename T>
  requires is_char_v<T>
  inline bool isSpace(const T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = strlen(str, len);
    return max > 0
        ? impl_string::isSpace(str, str + max)
        : false;
  }

  ////// Remove pattern from string... ///////////////////////////////////////

  namespace impl_string {

    template<typename T>
    requires is_char_v<T>
    inline void removeAll(T *first, T* last,
                          const T *pat, const std::size_t maxpat)
    {
      T *end = last;
      for(T *hit = first;
          (hit = std::search(hit, end, pat, pat + maxpat)) != end;
          ) {
        end = std::copy(hit + maxpat, end, hit);
      }
      std::for_each(end, last, lambda_set_null<T>());
    }

  } // namespace impl_string

  template<typename T>
  requires is_char_v<T>
  inline void removeAll(T *first, T* last,
                        const T *pat, const std::size_t lenpat = MAX_SIZE_T)
  {
    const std::size_t maxstr = strlen(first, last);
    const std::size_t maxpat = strlen(pat, lenpat);
    if( maxstr > 0  &&  maxpat > 0  &&  maxstr >= maxpat ) {
      impl_string::removeAll(first, last, pat, maxpat);
    }
  }

  template<typename T>
  requires is_char_v<T>
  inline void removeAll(T *str, const std::size_t lenstr,
                        const T *pat, const std::size_t lenpat = MAX_SIZE_T)
  {
    const std::size_t maxstr = strlen(str, lenstr);
    const std::size_t maxpat = strlen(pat, lenpat);
    if( maxstr > 0  &&  maxpat > 0  &&  maxstr >= maxpat ) {
      impl_string::removeAll(str, str + maxstr, pat, maxpat);
    }
  }

  template<typename T>
  requires is_char_v<T>
  inline void removeAll(T *str,
                        const T *pat, const std::size_t lenpat = MAX_SIZE_T)
  {
    removeAll(str, MAX_SIZE_T, pat, lenpat);
  }

  ////// Remove character from string... /////////////////////////////////////

  namespace impl_string {

    template<typename T>
    requires is_char_v<T>
    inline void removeAll(T *first, T *last, const T& pat)
    {
      T *end = std::remove(first, last, pat);
      std::for_each(end, last, lambda_set_null<T>());
    }

  } // namespace impl_string

  template<typename T>
  requires is_char_v<T>
  inline void removeAll(T *first, T *last, const T& pat)
  {
    if( Pointer::isValidRange(first, last) ) {
      impl_string::removeAll(first, last, pat);
    }
  }

  template<typename T>
  requires is_char_v<T>
  inline void removeAll(T *str, const std::size_t len, const T& pat)
  {
    const std::size_t max = strlen(str, len);
    if( max > 0 ) {
      impl_string::removeAll(str, str + max, pat);
    }
  }

  template<typename T>
  requires is_char_v<T>
  inline void removeAll(T *str, const T& pat)
  {
    removeAll(str, MAX_SIZE_T, pat);
  }

  ////// Remove character matching predicate from string... //////////////////

  namespace impl_string {

    template<typename T, typename PredFunc>
    requires is_char_v<T>
    inline void removeAll(T *first, T *last, PredFunc func)
    {
      T *end = std::remove_if(first, last, func);
      std::for_each(end, last, lambda_set_null<T>());
    }

  } // namespace impl_string

  template<typename T, typename PredFunc>
  requires is_char_v<T>
  inline void removeAll(T *first, T *last, PredFunc func)
  {
    if( Pointer::isValidRange(first, last) ) {
      impl_string::removeAll<T,PredFunc>(first, last, func);
    }
  }

  template<typename T, typename PredFunc>
  requires is_char_v<T>
  inline void removeAll(T *str, const std::size_t len, PredFunc func)
  {
    const std::size_t max = strlen(str, len);
    if( max > 0 ) {
      impl_string::removeAll<T,PredFunc>(str, str + max, func);
    }
  }

  template<typename T, typename PredFunc>
  requires is_char_v<T>
  inline void removeAll(T *str, PredFunc func)
  {
    removeAll<T,PredFunc>(str, MAX_SIZE_T, func);
  }

  ////// Remove trailing zeros from Fixed-Notation Floating-Point string... //

  namespace impl_string {

    template<typename T>
    requires is_char_v<T>
    inline void removeTrailingZeros(T *first, T *last, const bool removeDot)
    {
      using RevIter = std::reverse_iterator<T*>;

      using g = glyph<T>;

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

  } // namespace impl_string

  template<typename T>
  requires is_char_v<T>
  inline void removeTrailingZeros(T *first, T *last,
                                  const bool removeDot = true)
  {
    if( Pointer::isValidRange(first, last) ) {
      impl_string::removeTrailingZeros(first, last, removeDot);
    }
  }

  template<typename T>
  requires is_char_v<T>
  inline void removeTrailingZeros(T *str, const std::size_t len,
                                  const bool removeDot = true)
  {
    const std::size_t max = strlen(str, len);
    if( max > 0 ) {
      impl_string::removeTrailingZeros(str, str + max, removeDot);
    }
  }

  template<typename T>
  requires is_char_v<T>
  inline void removeTrailingZeros(T *str,
                                  const bool removeDot = true)
  {
    removeTrailingZeros(str, MAX_SIZE_T, removeDot);
  }

  ////// Replace character in string... //////////////////////////////////////

  namespace impl_string {

    template<typename T>
    requires is_char_v<T>
    inline void replaceAll(T *first, T *last, const T& pat, const T& txt)
    {
      std::replace(first, last, pat, txt);
    }

  } // namespace impl_string

  template<typename T>
  requires is_char_v<T>
  inline void replaceAll(T *first, T *last, const T& pat, const T& txt)
  {
    if( Pointer::isValidRange(first, last, pat, txt) ) {
      impl_string::replaceAll(first, last, pat, txt);
    }
  }

  template<typename T>
  requires is_char_v<T>
  inline void replaceAll(T *str, const std::size_t len,
                         const T& pat, const T& txt)
  {
    const std::size_t max = strlen(str, len);
    if( max > 0 ) {
      impl_string::replaceAll(str, str + max, pat, txt);
    }
  }

  template<typename T>
  requires is_char_v<T>
  inline void replaceAll(T *str,
                         const T& pat, const T& txt)
  {
    replaceAll(str, MAX_SIZE_T, pat, txt);
  }

  ////// Replace character matching predicate in string... ///////////////////

  namespace impl_string {

    template<typename T, typename PredFunc>
    requires is_char_v<T>
    inline void replaceAll(T *first, T *last, PredFunc func, const T& txt)
    {
      std::replace_if(first, last, func, txt);
    }

  } // namespace impl_string

  template<typename T, typename PredFunc>
  requires is_char_v<T>
  inline void replaceAll(T *first, T *last, PredFunc func, const T& txt)
  {
    if( Pointer::isValidRange(first, last) ) {
      impl_string::replaceAll<T,PredFunc>(first, last, func, txt);
    }
  }

  template<typename T, typename PredFunc>
  requires is_char_v<T>
  inline void replaceAll(T *str, const std::size_t len,
                         PredFunc func, const T& txt)
  {
    const std::size_t max = strlen(str, len);
    if( max > 0 ) {
      impl_string::replaceAll<T,PredFunc>(str, str + max, func, txt);
    }
  }

  template<typename T, typename PredFunc>
  requires is_char_v<T>
  inline void replaceAll(T *str,
                         PredFunc func, const T& txt)
  {
    replaceAll<T,PredFunc>(str, MAX_SIZE_T, func, txt);
  }

  ////// Remove whitespace from begin & end of string... /////////////////////

  namespace impl_string {

    template<typename T>
    requires is_char_v<T>
    inline void trim(T *first, T *last)
    {
      using RevIter = std::reverse_iterator<T*>;

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

  } // namespace impl_string

  template<typename T>
  requires is_char_v<T>
  inline void trim(T *first, T *last)
  {
    if( Pointer::isValidRange(first, last) ) {
      impl_string::trim(first, last);
    }
  }

  template<typename T>
  requires is_char_v<T>
  inline void trim(T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = strlen(str, len);
    if( max > 0 ) {
      impl_string::trim(str, str + max);
    }
  }

  ////// Replace consecutive whitespace with single space... /////////////////

  namespace impl_string {

    template<typename T>
    requires is_char_v<T>
    inline void simplify(T *first, T *last)
    {
      constexpr auto lambda_adjacent_space = [](const T& a, const T& b) -> bool {
        return ::cs::isSpace(a)  &&  ::cs::isSpace(b);
      };

      // (1) remove duplicate whitespace
      T *end = std::unique(first, last, lambda_adjacent_space);
      // (1.5) fill with NULL
      std::for_each(end, last, lambda_set_null<T>());
      // (2) replace single whitespace characters with space
      std::replace_if(first, end, lambda_is_space<T>(), glyph<T>::space);
      // (3) trim result
      impl_string::trim(first, end);
    }

  } // namespace impl_string

  template<typename T>
  requires is_char_v<T>
  inline void simplify(T *first, T *last)
  {
    if( Pointer::isValidRange(first, last) ) {
      impl_string::simplify(first, last);
    }
  }

  template<typename T>
  requires is_char_v<T>
  inline void simplify(T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = strlen(str, len);
    if( max > 0 ) {
      impl_string::simplify(str, str + max);
    }
  }

  ////// String starts with pattern... ///////////////////////////////////////

  template<typename T>
  requires is_char_v<T>
  inline bool startsWith(const T *str, const std::size_t lenstr, // haystack
                         const T *pat, const std::size_t lenpat, // needle
                         const bool ignoreCase = false)
  {
    const std::size_t maxstr = strlen(str, lenstr);
    const std::size_t maxpat = strlen(pat, lenpat);

    if( maxstr > 0  &&  maxpat > 0  &&  maxstr >= maxpat ) {
      return ignoreCase
          ? std::equal(str, str + maxpat, pat, lambda_eqI<T>())
          : std::equal(str, str + maxpat, pat);
    }

    return false;
  }

  template<typename T>
  requires is_char_v<T>
  inline bool startsWith(const T *str, const T *pat, const bool ignoreCase = false)
  {
    return startsWith(str, MAX_SIZE_T, pat, MAX_SIZE_T, ignoreCase);
  }

  ////// Convert to lower case... ////////////////////////////////////////////

  namespace impl_string {

    template<typename T>
    requires is_char_v<T>
    inline void toLower(T *first, T *last)
    {
      std::for_each(first, last, lambda_to_lower<T>());
    }

  } // namespace impl_string

  template<typename T>
  requires is_char_v<T>
  inline void toLower(T *first, T *last)
  {
    if( Pointer::isValidRange(first, last) ) {
      impl_string::toLower(first, last);
    }
  }

  template<typename T>
  requires is_char_v<T>
  inline void toLower(T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = strlen(str, len);
    if( max > 0 ) {
      impl_string::toLower(str, str + max);
    }
  }

  ////// Convert to upper case... ////////////////////////////////////////////

  namespace impl_string {

    template<typename T>
    requires is_char_v<T>
    inline void toUpper(T *first, T *last)
    {
      std::for_each(first, last, lambda_to_upper<T>());
    }

  } // namespace impl_string

  template<typename T>
  requires is_char_v<T>
  inline void toUpper(T *first, T *last)
  {
    if( Pointer::isValidRange(first, last) ) {
      impl_string::toUpper(first, last);
    }
  }

  template<typename T>
  requires is_char_v<T>
  inline void toUpper(T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = strlen(str, len);
    if( max > 0 ) {
      impl_string::toUpper(str, str + max);
    }
  }

  ////// Widen string... /////////////////////////////////////////////////////

  namespace impl_string {

    template<typename WCharT, typename NCharT>
    requires is_widechar_v<WCharT>  &&  is_narrowchar_v<NCharT>
    inline void widen(WCharT *dest, const NCharT *first, const NCharT *last)
    {
      constexpr auto lambda_widen = [](const NCharT& c) -> WCharT {
        return static_cast<WCharT>(c);
      };

      std::transform(first, last, dest, lambda_widen);
    }

  } // namespace impl_string

  template<typename WCharT, typename NCharT>
  requires is_widechar_v<WCharT>  &&  is_narrowchar_v<NCharT>
  inline void widen(WCharT *dest, const NCharT *first, const NCharT *last)
  {
    if( dest != nullptr  &&  Pointer::isValidRange(first, last) ) {
      impl_string::widen(dest, first, last);
    }
  }

  template<typename WCharT, typename NCharT>
  requires is_widechar_v<WCharT>  &&  is_narrowchar_v<NCharT>
  inline void widen(WCharT *dest, const NCharT *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = strlen(str, len);
    if( dest != nullptr  &&  max > 0 ) {
      impl_string::widen(dest, str, str + max);
    }
  }

} // namespace cs
