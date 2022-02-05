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

#include <csUtil/csConcepts.h>
#include <csUtil/csMath.h>

namespace cs {

  template<typename T> requires IsReal<T>
  inline constexpr T INVALID_RESULT = std::numeric_limits<T>::quiet_NaN();

  namespace impl {

    inline bool isCount(const std::size_t count)
    {
      static_assert( sizeof(std::size_t) >= 4 );

      constexpr std::size_t MIN = 2;
      constexpr std::size_t MAX = 0x7FFFFFFF;

      return MIN <= count  &&  count <= MAX;
    }

    template<typename T, bool MINUS_ONE = false> requires IsReal<T>
    inline T normFact(const std::size_t count) // NOTE: Does NOT check 'count'!
    {
      static_assert( sizeof(double) == 8 );

      constexpr double ONE = 1;

      const double N = static_cast<double>(count);
      const double f = MINUS_ONE
          ? ONE/(N - ONE)
          : ONE/ N;

      return static_cast<T>(f);
    }

  } // namespace impl

  template<typename T> requires IsReal<T>
  inline T mean(const T *x, const std::size_t N)
  {
    if( x == nullptr  ||  !impl::isCount(N) ) {
      return INVALID_RESULT<T>;
    }

    const T fac = impl::normFact<T>(N);

    T sum = 0;
    for(std::size_t i = 0; i < N; i++) {
      sum += x[i];
    }

    return fac*sum; // Mean := mu
  }

  template<typename T> requires IsReal<T>
  inline T cov(const T *x, const T *y, const std::size_t N, const T muX, const T muY)
  {
    if( x == nullptr  ||  y == nullptr  ||  !impl::isCount(N)  ||
        isNaN(muX)  ||  isNaN(muY) ) {
      return INVALID_RESULT<T>;
    }

    const T fac = impl::normFact<T,true>(N);

    T sum = 0;
    for(std::size_t i = 0; i < N; i++) {
      const T dX = x[i] - muX;
      const T dY = y[i] - muY;
      sum += dX*dY;
    }

    return fac*sum;
  }

  template<typename T> requires IsReal<T>
  inline T cov(const T *x, const T *y, const std::size_t N)
  {
    return cov(x, y, N, mean(x, N), mean(y, N));
  }

  template<typename T> requires IsReal<T>
  inline T var(const T *x, const std::size_t N, const T mu)
  {
    if( x == nullptr  ||  !impl::isCount(N)  ||
        isNaN(mu) ) {
      return INVALID_RESULT<T>;
    }

    const T fac = impl::normFact<T,true>(N);

    T sum = 0;
    for(std::size_t i = 0; i < N; i++) {
      const T d = x[i] - mu;
      sum += d*d;
    }

    return fac*sum;
  }

  template<typename T> requires IsReal<T>
  inline T var(const T *x, const std::size_t N)
  {
    return var(x, N, mean(x, N));
  }

  template<typename T> requires IsReal<T>
  inline T stddev(const T *x, const std::size_t N, const T mu)
  {
    const T ss = var(x, N, mu); // Variance := s*s
    if( isNaN(ss) ) {
      return INVALID_RESULT<T>;
    }
    return sqrt(ss); // Standard Deviation := s (AKA "sigma")
  }

  template<typename T> requires IsReal<T>
  inline T stddev(const T *x, const std::size_t N)
  {
    return stddev(x, N, mean(x, N));
  }

  template<typename T> requires IsReal<T>
  inline T corr(const T *x, const T *y, const std::size_t N, const T muX, const T muY)
  {
    if( x == nullptr  ||  y == nullptr  ||  !impl::isCount(N)  ||
        isNaN(muX)  ||  isNaN(muY) ) {
      return INVALID_RESULT<T>;
    }

    const T   covXY = cov(x, y, N, muX, muY);
    const T sigmaX  = stddev(x, N, muX);
    const T sigmaY  = stddev(y, N, muY);

    return covXY/(sigmaX*sigmaY); // Pearson Correlation Coefficient := r (AKA "rho")
  }

  template<typename T> requires IsReal<T>
  inline T corr(const T *x, const T *y, const std::size_t N)
  {
    return corr(x, y, N, mean(x, N), mean(y, N));
  }

} // namespace cs

#endif // CSSTATISTICS_H
