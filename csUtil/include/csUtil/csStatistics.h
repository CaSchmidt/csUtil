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

#include <limits>
#include <numeric>
#include <type_traits>

#include <csUtil/csMath.h>

namespace cs {

  template<typename T>
  using if_real_t = std::enable_if_t<std::is_floating_point_v<T>,T>;

  template<typename T>
  inline constexpr if_real_t<T> INVALID_RESULT = std::numeric_limits<T>::quiet_NaN();

  namespace impl {

    inline bool isCount(const std::size_t count)
    {
      static_assert( sizeof(std::size_t) >= 4 );

      constexpr std::size_t ONE = 1;
      constexpr std::size_t MAX = 0x7FFFFFFF;

      return ONE <= count  &&  count <= MAX;
    }

    template<typename T, bool MINUS_ONE = false>
    inline if_real_t<T> normFact(const std::size_t count) // NOTE: Does NOT check 'count'!
    {
      constexpr double ONE = 1;

      const double N = static_cast<double>(count);
      const double f = MINUS_ONE
          ? ONE/(N - ONE)
          : ONE/ N;

      return static_cast<T>(f);
    }

  } // namespace impl

  template<typename T>
  if_real_t<T> mean(const T *data, const std::size_t count)
  {
    constexpr T ZERO = 0;

    if( !impl::isCount(count) ) {
      return INVALID_RESULT<T>;
    }

    const T sum = std::accumulate<const T*,T>(data, data + count, ZERO);
    const T fac = impl::normFact<T>(count);

    return sum*fac;
  }

} // namespace cs

#endif // CSSTATISTICS_H
