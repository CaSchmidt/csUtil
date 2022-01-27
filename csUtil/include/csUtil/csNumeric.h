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

#ifndef CSNUMERIC_H
#define CSNUMERIC_H

#include <csUtil/csConcepts.h>

namespace cs {

  template<typename T, T BASE = 10> requires IsIntegral<T>
  inline std::size_t countDigits(T x)
  {
    static_assert( 2 <= BASE  &&  BASE <= 36 );

    constexpr T ZERO = 0;

    std::size_t cnt = 0;
    do {
      cnt++;
      x /= BASE;
    } while( x != ZERO );
    return cnt;
  }

  template<typename T> requires IsIntegral<T>
  constexpr bool isEven(const T& x)
  {
    constexpr T  ONE = 1;
    constexpr T ZERO = 0;

    return (x & ONE) == ZERO;
  }

  template<typename T> requires IsIntegral<T>
  constexpr bool isOdd(const T& x)
  {
    constexpr T  ONE = 1;
    constexpr T ZERO = 0;

    return (x & ONE) != ZERO;
  }

} // namespace cs

#endif // CSNUMERIC_H
