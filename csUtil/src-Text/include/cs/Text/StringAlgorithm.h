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

#include <algorithm>

#include <cs/Core/CharUtil.h>
#include <cs/Core/Range.h>

namespace cs {

  ////// String is hexadecimal string... /////////////////////////////////////

  namespace impl_string {

    template<typename T> requires is_char_v<T>
    inline bool isHexString(const T *first, const T *last)
    {
      return std::all_of(first, last, lambda_is_hex<T>());
    }

  } // namespace impl_string

  template<typename T> requires is_char_v<T>
  inline bool isHexString_s(const T *str, const std::size_t len)
  {
    const std::size_t max = strlen(str, len);

    return max > 0
        ? impl_string::isHexString(str, str + max)
        : false;
  }

  template<typename T> requires is_char_v<T>
  inline bool isHexString(const T *str, const std::size_t len)
  {
    return impl_string::isHexString(str, str + len);
  }

  template<typename T> requires is_char_v<T>
  inline bool isHexString(const T *str)
  {
    const std::size_t max = strlen(str);

    return max > 0
        ? impl_string::isHexString(str, str + max)
        : false;
  }

  ////// String is C-style identifier... /////////////////////////////////////

  namespace impl_string {

    template<typename T> requires is_char_v<T>
    inline bool isIdent(const T *first, const T *last)
    {
      return isIdentFirst(*first)  &&
          std::all_of(first + 1, last, lambda_is_ident<T>());
    }

  } // namespace impl_string

  template<typename T> requires is_char_v<T>
  inline bool isIdent_s(const T *str, const std::size_t len)
  {
    const std::size_t max = strlen(str, len);

    return max > 0
        ? impl_string::isIdent(str, str + max)
        : false;
  }

  template<typename T> requires is_char_v<T>
  inline bool isIdent(const T *str, const std::size_t len)
  {
    return impl_string::isIdent(str, str + len);
  }

  template<typename T> requires is_char_v<T>
  inline bool isIdent(const T *str)
  {
    const std::size_t max = strlen(str);

    return max > 0
        ? impl_string::isIdent(str, str + max)
        : false;
  }

  ////// String contains only whitespace... //////////////////////////////////

  namespace impl_string {

    template<typename T> requires is_char_v<T>
    inline bool isSpace(const T *first, const T *last)
    {
      return std::all_of(first, last, lambda_is_space<T>());
    }

  } // namespace impl_string

  template<typename T> requires is_char_v<T>
  inline bool isSpace_s(const T *str, const std::size_t len)
  {
    const std::size_t max = strlen(str, len);

    return max > 0
        ? impl_string::isSpace(str, str + max)
        : false;
  }

  template<typename T> requires is_char_v<T>
  inline bool isSpace(const T *str, const std::size_t len)
  {
    return impl_string::isSpace(str, str + len);
  }

  template<typename T> requires is_char_v<T>
  inline bool isSpace(const T *str)
  {
    const std::size_t max = strlen(str);

    return max > 0
        ? impl_string::isSpace(str, str + max)
        : false;
  }

  ////// Remove pattern from string... ///////////////////////////////////////

  namespace impl_string {

    template<typename T> requires is_char_v<T>
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

  template<typename T> requires is_char_v<T>
  inline void removeAll_s(T *str, const std::size_t lenstr,
                          const T *pat, const std::size_t lenpat)
  {
    const std::size_t maxstr = strlen(str, lenstr);
    const std::size_t maxpat = strlen(pat, lenpat);

    if( maxstr > 0  &&  maxstr >= maxpat ) {
      impl_string::removeAll(str, str + maxstr, pat, maxpat);
    }
  }

  template<typename T> requires is_char_v<T>
  inline void removeAll(T *str, const std::size_t lenstr,
                        const T *pat, const std::size_t lenpat)
  {
    if( lenstr >= lenpat ) {
      impl_string::removeAll(str, str + lenstr, pat, lenpat);
    }
  }

  template<typename T> requires is_char_v<T>
  inline void removeAll(T *str,
                        const T *pat)
  {
    const std::size_t maxstr = strlen(str);
    const std::size_t maxpat = strlen(pat);

    if( maxstr > 0  &&  maxstr >= maxpat ) {
      impl_string::removeAll(str, str + maxstr, pat, maxpat);
    }
  }

  ////// Remove character from string... /////////////////////////////////////

  namespace impl_string {

    template<typename T> requires is_char_v<T>
    inline void removeAll(T *first, T *last,
                          const T pat)
    {
      T *end = std::remove(first, last, pat);
      std::for_each(end, last, lambda_set_null<T>());
    }

  } // namespace impl_string

  template<typename T> requires is_char_v<T>
  inline void removeAll_s(T *str, const std::size_t len,
                          const T pat)
  {
    const std::size_t max = strlen(str, len);

    if( max > 0 ) {
      impl_string::removeAll(str, str + max, pat);
    }
  }

  template<typename T> requires is_char_v<T>
  inline void removeAll(T *str, const std::size_t len,
                        const T pat)
  {
    impl_string::removeAll(str, str + len, pat);
  }

  template<typename T> requires is_char_v<T>
  inline void removeAll(T *str,
                        const T pat)
  {
    const std::size_t max = strlen(str);

    if( max > 0 ) {
      impl_string::removeAll(str, str + max, pat);
    }
  }

  ////// Remove character matching predicate from string... //////////////////

  namespace impl_string {

    template<typename T, typename PredFunc> requires is_char_v<T>
    inline void removeAll(T *first, T *last,
                          PredFunc func)
    {
      T *end = std::remove_if(first, last, func);
      std::for_each(end, last, lambda_set_null<T>());
    }

  } // namespace impl_string

  template<typename T, typename PredFunc> requires is_char_v<T>
  inline void removeAll_s(T *str, const std::size_t len,
                          PredFunc func)
  {
    const std::size_t max = strlen(str, len);

    if( max > 0 ) {
      impl_string::removeAll<T,PredFunc>(str, str + max, func);
    }
  }

  template<typename T, typename PredFunc> requires is_char_v<T>
  inline void removeAll(T *str, const std::size_t len,
                        PredFunc func)
  {
    impl_string::removeAll<T,PredFunc>(str, str + len, func);
  }

  template<typename T, typename PredFunc> requires is_char_v<T>
  inline void removeAll(T *str,
                        PredFunc func)
  {
    const std::size_t max = strlen(str);

    if( max > 0 ) {
      impl_string::removeAll<T,PredFunc>(str, str + max, func);
    }
  }

  ////// Remove trailing zeros from Fixed-Notation Floating-Point string... //

  namespace impl_string {

    template<typename T> requires is_char_v<T>
    inline void removeTrailingZeros(T *first, T *last,
                                    const bool removeDot)
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

      T *end = std::find_if_not(RevIter(last), RevIter(dot),
                                lambda_is_zero<T>()).base();
      std::for_each(end, last, lambda_set_null<T>());

      if( removeDot  &&  end == dot + 1 ) {
        *dot = g::NUL;
      }
    }

  } // namespace impl_string

  template<typename T> requires is_char_v<T>
  inline void removeTrailingZeros_s(T *str, const std::size_t len,
                                    const bool removeDot = true)
  {
    const std::size_t max = strlen(str, len);

    if( max > 0 ) {
      impl_string::removeTrailingZeros(str, str + max, removeDot);
    }
  }

  template<typename T> requires is_char_v<T>
  inline void removeTrailingZeros(T *str, const std::size_t len,
                                  const bool removeDot = true)
  {
    impl_string::removeTrailingZeros(str, str + len, removeDot);
  }

  template<typename T> requires is_char_v<T>
  inline void removeTrailingZeros(T *str,
                                  const bool removeDot = true)
  {
    const std::size_t max = strlen(str);

    if( max > 0 ) {
      impl_string::removeTrailingZeros(str, str + max, removeDot);
    }
  }

  ////// Remove whitespace from begin & end of string... /////////////////////

  namespace impl_string {

    template<typename T> requires is_char_v<T>
    inline void trim(T *first, T *last)
    {
      using RevIter = std::reverse_iterator<T*>;

      // trim left
      T *lend = std::copy(std::find_if_not(first, last,
                                           lambda_is_space<T>()),
                          last,
                          first);
      // trim right
      T *rend = std::find_if_not(RevIter(lend), RevIter(first),
                                 lambda_is_space<T>()).base();
      // fill with NULL
      std::for_each(rend, last, lambda_set_null<T>());
    }

  } // namespace impl_string

  template<typename T> requires is_char_v<T>
  inline void trim_s(T *str, const std::size_t len)
  {
    const std::size_t max = strlen(str, len);

    if( max > 0 ) {
      impl_string::trim(str, str + max);
    }
  }

  template<typename T> requires is_char_v<T>
  inline void trim(T *str, const std::size_t len)
  {
    impl_string::trim(str, str + len);
  }

  template<typename T> requires is_char_v<T>
  inline void trim(T *str)
  {
    const std::size_t max = strlen(str);

    if( max > 0 ) {
      impl_string::trim(str, str + max);
    }
  }

  ////// Replace consecutive whitespace with single space... /////////////////

  namespace impl_string {

    template<typename T> requires is_char_v<T>
    inline void simplify(T *first, T *last)
    {
      constexpr auto lambda_adjacent_space = [](const T a, const T b) -> bool {
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

  template<typename T> requires is_char_v<T>
  inline void simplify_s(T *str, const std::size_t len)
  {
    const std::size_t max = strlen(str, len);

    if( max > 0 ) {
      impl_string::simplify(str, str + max);
    }
  }

  template<typename T> requires is_char_v<T>
  inline void simplify(T *str, const std::size_t len)
  {
    impl_string::simplify(str, str + len);
  }

  template<typename T> requires is_char_v<T>
  inline void simplify(T *str)
  {
    const std::size_t max = strlen(str);

    if( max > 0 ) {
      impl_string::simplify(str, str + max);
    }
  }

  ////// Skip leading whitespace /////////////////////////////////////////////

  namespace impl_string {

    template<typename T> requires is_char_v<T>
    inline const T *skipSpace(const T *first, const T *last)
    {
      return std::find_if_not(first, last, lambda_is_space<T>());
    }

  } // namespace impl_string

  template<typename T> requires is_char_v<T>
  inline const T *skipSpace_s(const T *str, const std::size_t len)
  {
    const std::size_t max = strlen(str, len);

    return max > 0
        ? impl_string::skipSpace(str, str + max)
        : nullptr;
  }

  template<typename T> requires is_char_v<T>
  inline const T *skipSpace(const T *str, const std::size_t len)
  {
    return impl_string::skipSpace(str, str + len);
  }

  template<typename T> requires is_char_v<T>
  inline const T *skipSpace(const T *str)
  {
    const std::size_t max = strlen(str);

    return max > 0
        ? impl_string::skipSpace(str, str + max)
        : nullptr;
  }

  ////// Convert to lower case... ////////////////////////////////////////////

  namespace impl_string {

    template<typename T> requires is_char_v<T>
    inline void toLower(T *first, T *last)
    {
      std::for_each(first, last, lambda_to_lower<T>());
    }

  } // namespace impl_string

  template<typename T> requires is_char_v<T>
  inline void toLower_s(T *str, const std::size_t len)
  {
    const std::size_t max = strlen(str, len);

    if( max > 0 ) {
      impl_string::toLower(str, str + max);
    }
  }

  template<typename T> requires is_char_v<T>
  inline void toLower(T *str, const std::size_t len)
  {
    impl_string::toLower(str, str + len);
  }

  template<typename T> requires is_char_v<T>
  inline void toLower(T *str)
  {
    const std::size_t max = strlen(str);

    if( max > 0 ) {
      impl_string::toLower(str, str + max);
    }
  }

  ////// Convert to upper case... ////////////////////////////////////////////

  namespace impl_string {

    template<typename T> requires is_char_v<T>
    inline void toUpper(T *first, T *last)
    {
      std::for_each(first, last, lambda_to_upper<T>());
    }

  } // namespace impl_string

  template<typename T> requires is_char_v<T>
  inline void toUpper_s(T *first, T *last)
  {
    const std::size_t max = strlen(first, last);

    if( max > 0 ) {
      impl_string::toUpper(first, first + max);
    }
  }

  template<typename T> requires is_char_v<T>
  inline void toUpper_s(T *str, const std::size_t len)
  {
    const std::size_t max = strlen(str, len);

    if( max > 0 ) {
      impl_string::toUpper(str, str + max);
    }
  }

  template<typename T> requires is_char_v<T>
  inline void toUpper(T *str, const std::size_t len)
  {
    impl_string::toUpper(str, str + len);
  }

  template<typename T> requires is_char_v<T>
  inline void toUpper(T *str)
  {
    const std::size_t max = strlen(str);

    if( max > 0 ) {
      impl_string::toUpper(str, str + max);
    }
  }

  ////// Narrow/Widen string... //////////////////////////////////////////////

  namespace impl_string {

    template<typename NCharT, typename WCharT>
    requires is_narrowchar_v<NCharT>  &&  is_widechar_v<WCharT>
    inline void narrow(NCharT *dest,
                       const WCharT *first, const WCharT *last)
    {
      std::transform(first, last, dest, lambda_narrow<NCharT,WCharT>());
    }

    template<typename WCharT, typename NCharT>
    requires is_widechar_v<WCharT>  &&  is_narrowchar_v<NCharT>
    inline void widen(WCharT *dest,
                      const NCharT *first, const NCharT *last)
    {
      std::transform(first, last, dest, lambda_widen<WCharT,NCharT>());
    }

  } // namespace impl_string

} // namespace cs
