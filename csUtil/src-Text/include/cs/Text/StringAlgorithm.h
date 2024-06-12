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
