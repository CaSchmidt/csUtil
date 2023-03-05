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

#include <cs/Core/Concepts.h>

namespace cs {

  ////// Glyphs //////////////////////////////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  struct glyph {
    static constexpr auto null = static_cast<T>('\0');

    static constexpr auto space = static_cast<T>(' ');  // space
    static constexpr auto    ff = static_cast<T>('\f'); // form feed
    static constexpr auto    lf = static_cast<T>('\n'); // line feed
    static constexpr auto    cr = static_cast<T>('\r'); // carriage return
    static constexpr auto    ht = static_cast<T>('\t'); // horizontal tab
    static constexpr auto    vt = static_cast<T>('\v'); // vertical tab

    static constexpr auto zero = static_cast<T>('0');
    static constexpr auto nine = static_cast<T>('9');

    static constexpr auto a = static_cast<T>('a');
    static constexpr auto e = static_cast<T>('e');
    static constexpr auto f = static_cast<T>('f');
    static constexpr auto z = static_cast<T>('z');

    static constexpr auto A = static_cast<T>('A');
    static constexpr auto E = static_cast<T>('E');
    static constexpr auto F = static_cast<T>('F');
    static constexpr auto Z = static_cast<T>('Z');

    static constexpr auto   dot = static_cast<T>('.');
    static constexpr auto   pct = static_cast<T>('%');
    static constexpr auto under = static_cast<T>('_');
  };

  ////// Classification //////////////////////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  constexpr bool isDigit(const T& c)
  {
    return glyph<T>::zero <= c  &&  c <= glyph<T>::nine;
  }

  template<typename T> requires IsCharacter<T>
  inline bool isHexDigit(const T& c)
  {
    const bool is_0to9 = glyph<T>::zero <= c  &&  c <= glyph<T>::nine;
    const bool is_atof = glyph<T>::a    <= c  &&  c <= glyph<T>::f;
    const bool is_AtoF = glyph<T>::A    <= c  &&  c <= glyph<T>::F;
    return is_0to9  ||  is_atof  ||  is_AtoF;
  }

  template<typename T> requires IsCharacter<T>
  constexpr bool isLower(const T& c)
  {
    return glyph<T>::a <= c  &&  c <= glyph<T>::z;
  }

  template<typename T> requires IsCharacter<T>
  constexpr bool isSpace(const T& c)
  {
    return
        c == glyph<T>::space  ||
        c == glyph<T>::ff     ||
        c == glyph<T>::lf     ||
        c == glyph<T>::cr     ||
        c == glyph<T>::ht     ||
        c == glyph<T>::vt;
  }

  template<typename T> requires IsCharacter<T>
  constexpr bool isUpper(const T& c)
  {
    return glyph<T>::A <= c  &&  c <= glyph<T>::Z;
  }

  template<typename T, bool IS_FIRST = false> requires IsCharacter<T>
  constexpr bool isIdent(const T& c)
  {
    return isLower(c)  ||  isUpper(c)  ||  (!IS_FIRST  &&  isDigit(c))  ||  c == glyph<T>::under;
  }

  template<typename T> requires IsCharacter<T>
  constexpr bool isIdentFirst(const T& c)
  {
    return isIdent<T,true>(c);
  }

  ////// Case Conversion /////////////////////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  constexpr T toLower(const T& c)
  {
    return isUpper(c)
        ? c - glyph<T>::A + glyph<T>::a
        : c;
  }

  template<typename T> requires IsCharacter<T>
  constexpr T toUpper(const T& c)
  {
    return isLower(c)
        ? c - glyph<T>::a + glyph<T>::A
        : c;
  }

  ////// Hex Conversion //////////////////////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  constexpr uint8_t fromHexChar(const T c)
  {
    using g = glyph<T>;
    if(        g::a    <= c  &&  c <= g::f    ) {
      return c - g::a + 10;
    } else if( g::A    <= c  &&  c <= g::F    ) {
      return c - g::A + 10;
    } else if( g::zero <= c  &&  c <= g::nine ) {
      return c - g::zero;
    }
    return std::numeric_limits<T>::max();
  }

  template<typename T, bool UPPER = false> requires IsCharacter<T>
  constexpr T toHexChar(const uint8_t in, const bool hi_nibble = false)
  {
    constexpr T hex10 = UPPER
        ? glyph<T>::A
        : glyph<T>::a;
    const uint8_t nibble = hi_nibble
        ? in >>    4
        : in  & 0x0F;
    return nibble >= 10
        ? nibble - 10 + hex10
        : nibble + glyph<T>::zero;
  }

  ////// Lambdas /////////////////////////////////////////////////////////////

  template<typename T> requires IsCharacter<T>
  constexpr auto lambda_eqI()
  {
    return [](const T& a, const T& b) -> bool {
      return toLower(a) == toLower(b);
    };
  }

  template<typename T> requires IsCharacter<T>
  constexpr auto lambda_is_hex()
  {
    return [](const T& c) -> bool {
      return isHexDigit(c);
    };
  }

  template<typename T> requires IsCharacter<T>
  constexpr auto lambda_is_ident()
  {
    return [](const T& c) -> bool {
      return isIdent(c);
    };
  }

  template<typename T> requires IsCharacter<T>
  constexpr auto lambda_is_space()
  {
    return [](const T& c) -> bool {
      return isSpace(c);
    };
  }

  template<typename T> requires IsCharacter<T>
  constexpr auto lambda_is_zero()
  {
    return [](const T& c) -> bool {
      return c == glyph<T>::zero;
    };
  }

  template<typename T> requires IsCharacter<T>
  constexpr auto lambda_set_null()
  {
    return [](T& c) -> void {
      c = glyph<T>::null;
    };
  }

  template<typename T> requires IsCharacter<T>
  constexpr auto lambda_to_lower()
  {
    return [](T& c) -> void {
      c = toLower(c);
    };
  }

  template<typename T> requires IsCharacter<T>
  constexpr auto lambda_to_upper()
  {
    return [](T& c) -> void {
      c = toUpper(c);
    };
  }

} // namespace cs
