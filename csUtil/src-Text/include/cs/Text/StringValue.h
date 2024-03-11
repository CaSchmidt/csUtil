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

#pragma once

#include <cctype>

#include <algorithm>
#include <charconv>
#include <string>

#include <cs/Core/TypeTraits.h>

namespace cs {

  namespace impl_strval {

    // Types /////////////////////////////////////////////////////////////////

    using size_type = std::string_view::size_type;

    // Constants /////////////////////////////////////////////////////////////

    constexpr size_type ONE = 1;

    // Implementation ////////////////////////////////////////////////////////

    inline const char *skipSpace(const char *first, const char *last)
    {
      constexpr auto lambda = [](const char ch) -> bool {
        // cf. https://en.cppreference.com/w/cpp/string/byte/isspace
        return std::isspace(static_cast<unsigned char>(ch)) != 0;
      };

      return std::find_if_not(first, last, lambda);
    }

  } // namespace impl_strval

  ////// Public //////////////////////////////////////////////////////////////

  template<typename T, bool HAVE_SKIP = true>
  inline if_integral_t<T> toValue(const std::string_view& str,
                                  bool *ok = nullptr, const int base = 10)
  {
    using namespace impl_strval;

    constexpr T ERROR_VALUE = T{0};

    // (0) Initialization ////////////////////////////////////////////////////

    if( ok != nullptr ) {
      *ok = false;
    }

    if( str.empty() ) {
      return ERROR_VALUE;
    }

    const char *first = str.data();
    const char  *last = str.data() + str.size();

    // (1) Sanitize base /////////////////////////////////////////////////////

    if( base < 2  ||  base > 36 ) {
      return ERROR_VALUE;
    }

    // (2) Skip leading whitespace ///////////////////////////////////////////

    if constexpr( HAVE_SKIP ) {
      first = skipSpace(first, last);

      if( first == last ) {
        return ERROR_VALUE;
      }
    } // HAVE_SKIP

    // (3) Skip leading plus sign ////////////////////////////////////////////

    if constexpr( HAVE_SKIP ) {
      if( first[0] == '+' ) {
        first += ONE;
      }

      if( first == last ) {
        return ERROR_VALUE;
      }
    } // HAVE_SKIP

    // (4) Conversion ////////////////////////////////////////////////////////

    T value = ERROR_VALUE;
    const std::from_chars_result result = std::from_chars(first, last, value, base);
    if( result.ec != std::errc{} ) {
      return ERROR_VALUE;
    }

    // Done! /////////////////////////////////////////////////////////////////

    if( ok != nullptr ) {
      *ok = true;
    }

    return value;
  }

  template<typename T, bool HAVE_SKIP = true>
  inline if_real_t<T> toValue(const std::string_view& str,
                              bool *ok = nullptr, const int base = 10)
  {
    using namespace impl_strval;

    constexpr T ERROR_VALUE = T{0};

    // (0) Initialization ////////////////////////////////////////////////////

    if( ok != nullptr ) {
      *ok = false;
    }

    if( str.empty() ) {
      return ERROR_VALUE;
    }

    const char *first = str.data();
    const char  *last = str.data() + str.size();

    // (1) Sanitize base /////////////////////////////////////////////////////

    if( base != 10  &&  base != 16 ) {
      return ERROR_VALUE;
    }

    const std::chars_format fmt = base == 16
        ? std::chars_format::hex
        : std::chars_format::general;

    // (2) Skip leading whitespace ///////////////////////////////////////////

    if constexpr( HAVE_SKIP ) {
      first = skipSpace(first, last);

      if( first == last ) {
        return ERROR_VALUE;
      }
    } // HAVE_SKIP

    // (3) Skip leading plus sign ////////////////////////////////////////////

    if constexpr( HAVE_SKIP ) {
      if( first[0] == '+' ) {
        first += ONE;
      }

      if( first == last ) {
        return ERROR_VALUE;
      }
    } // HAVE_SKIP

    // (4) Conversion ////////////////////////////////////////////////////////

    T value = ERROR_VALUE;
    const std::from_chars_result result = std::from_chars(first, last, value, fmt);
    if( result.ec != std::errc{} ) {
      return ERROR_VALUE;
    }

    // Done! /////////////////////////////////////////////////////////////////

    if( ok != nullptr ) {
      *ok = true;
    }

    return value;
  }

} // namespace cs
