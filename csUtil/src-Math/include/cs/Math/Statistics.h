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

#pragma once

#include <cstddef>

#include <cs/Core/Constants.h>
#include <cs/Math/Math.h>
#include <cs/SIMD/SIMD.h>
#include <cs/SIMD/SIMD128.h>

namespace cs {

  namespace impl_statistics {

    template<typename T> requires IsReal<T>
    inline bool isCount(const std::size_t count)
    {
      using k = konst<std::size_t>;

      constexpr k::value_type MIN = k::TWO;
      constexpr k::value_type MAX = sizeof(k::value_type) >= sizeof(T)
          ? (k::ONE << std::numeric_limits<T>::digits) - k::ONE
          : k::MAX;

      return MIN <= count  &&  count <= MAX;
    }

  } // namespace impl_statistics

  template<typename T> requires IsReal<T>
  inline T mean(const T *x, const std::size_t count)
  {
    if( x == nullptr  ||  !impl_statistics::isCount<T>(count) ) {
      return real_konst<T>::INVALID_RESULT;
    }

    const T N = static_cast<T>(count);

    const T sum = simd::sum<simd128<T>>(x, count);

    return sum/N;
  }

  template<typename T> requires IsReal<T>
  inline T cov(const T *x, const T *y, const std::size_t count,
               const T _meanX = real_konst<T>::INVALID_RESULT,
               const T _meanY = real_konst<T>::INVALID_RESULT)
  {
    using k = konst<T>;

    if( x == nullptr  ||  y == nullptr  ||  !impl_statistics::isCount<T>(count) ) {
      return real_konst<T>::INVALID_RESULT;
    }

    const T     N = static_cast<T>(count);
    const T meanX = !math<T>::isNaN(_meanX)
        ? _meanX
        : mean(x, count);
    const T meanY = !math<T>::isNaN(_meanY)
        ? _meanY
        : mean(y, count);

    const T sum = simd::dot<simd128<T>>(x, y, count);

    return (sum - N*meanX*meanY)/(N - k::ONE);
  }

  template<typename T> requires IsReal<T>
  inline T var(const T *x, const std::size_t count,
               const T _meanX = real_konst<T>::INVALID_RESULT)
  {
    using k = konst<T>;

    if( x == nullptr  ||  !impl_statistics::isCount<T>(count) ) {
      return real_konst<T>::INVALID_RESULT;
    }

    const T     N = static_cast<T>(count);
    const T meanX = !math<T>::isNaN(_meanX)
        ? _meanX
        : mean(x, count);

    const T sum = simd::sum_squared<simd128<T>>(x, count);

    return (sum - N*meanX*meanX)/(N - k::ONE);
  }

  template<typename T> requires IsReal<T>
  inline T stddev(const T *x, const std::size_t count,
                  const T _meanX = real_konst<T>::INVALID_RESULT)
  {
    const T ss = var(x, count, _meanX);
    if( math<T>::isNaN(ss) ) {
      return real_konst<T>::INVALID_RESULT;
    }
    return math<T>::sqrt(ss);
  }

} // namespace cs
