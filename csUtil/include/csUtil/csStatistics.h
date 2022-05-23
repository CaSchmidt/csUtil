/****************************************************************************
** Copyright (c) 2021, Carsten Schmidt. All rights reserved.
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

#ifndef CSSTATISTICS_H
#define CSSTATISTICS_H

#include <cstddef>

#include <csUtil/csMath.h>
#include <csUtil/csSIMD.h>
#include <csUtil/csSIMD128.h>

namespace cs {

  template<typename T> requires IsReal<T>
  inline constexpr T INVALID_RESULT = std::numeric_limits<T>::quiet_NaN();

  namespace impl_statistics {

    template<typename T> requires IsReal<T>
    inline bool isCount(const std::size_t count)
    {
      constexpr std::size_t ONE = 1;

      constexpr std::size_t MIN = 2;
      constexpr std::size_t MAX = sizeof(std::size_t) >= sizeof(T)
          ? (ONE << std::numeric_limits<T>::digits) - ONE
          : std::numeric_limits<std::size_t>::max();

      return MIN <= count  &&  count <= MAX;
    }

  } // namespace impl_statistics

  template<typename T> requires IsReal<T>
  inline T mean(const T *x, const std::size_t count)
  {
    if( x == nullptr  ||  !impl_statistics::isCount<T>(count) ) {
      return INVALID_RESULT<T>;
    }

    const T N = static_cast<T>(count);

    const T sum = simd::sum<simd128<T>>(x, count);

    return sum/N;
  }

  template<typename T> requires IsReal<T>
  inline T cov(const T *x, const T *y, const std::size_t count,
               const T _meanX = INVALID_RESULT<T>,
               const T _meanY = INVALID_RESULT<T>)
  {
    constexpr T ONE = 1;

    if( x == nullptr  ||  y == nullptr  ||  !impl_statistics::isCount<T>(count) ) {
      return INVALID_RESULT<T>;
    }

    const T     N = static_cast<T>(count);
    const T meanX = !isNaN(_meanX)
        ? _meanX
        : mean(x, count);
    const T meanY = !isNaN(_meanY)
        ? _meanY
        : mean(y, count);

    const T sum = simd::dot<simd128<T>>(x, y, count);

    return (sum - N*meanX*meanY)/(N - ONE);
  }

  template<typename T> requires IsReal<T>
  inline T var(const T *x, const std::size_t count,
               const T _meanX = INVALID_RESULT<T>)
  {
    constexpr T ONE = 1;

    if( x == nullptr  ||  !impl_statistics::isCount<T>(count) ) {
      return INVALID_RESULT<T>;
    }

    const T     N = static_cast<T>(count);
    const T meanX = !isNaN(_meanX)
        ? _meanX
        : mean(x, count);

    const T sum = simd::sum_squared<simd128<T>>(x, count);

    return (sum - N*meanX*meanX)/(N - ONE);
  }

  template<typename T> requires IsReal<T>
  inline T stddev(const T *x, const std::size_t count,
                  const T _meanX = INVALID_RESULT<T>)
  {
    const T ss = var(x, count, _meanX);
    if( isNaN(ss) ) {
      return INVALID_RESULT<T>;
    }
    return sqrt(ss);
  }

} // namespace cs

#endif // CSSTATISTICS_H
