/****************************************************************************
** Copyright (c) 2020, Carsten Schmidt. All rights reserved.
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

#ifndef N4_MATH_H
#define N4_MATH_H

#include <math.h>

#include <N4/TypeTraits.h>

namespace n4 {

  inline real_t abs(const real_t x)
  {
    return ::fabsf(x);
  }

  inline real_t acos(const real_t x)
  {
    return ::acosf(x);
  }

  inline double atan2(const double y, const double x)
  {
    return ::atan2(y, x);
  }

  inline float atan2(const float y, const float x)
  {
    return ::atan2f(y, x);
  }

  inline real_t cos(const real_t x)
  {
    return ::cosf(x);
  }

  inline real_t cosPI2(const signed int i)
  {
    static const real_t tab[4] = { 1, 0, -1, 0 };
    return tab[i & 0x3];
  }

  inline real_t floor(const real_t x)
  {
    return ::floorf(x);
  }

  inline bool isFinite(const real_t x)
  {
    return isfinite(x) != 0;
  }

  inline bool isNaN(const real_t x)
  {
    return isnan(x) != 0;
  }

  inline bool isZero(const real_t x, const real_t epsilon0 = EPSILON0_SCALAR)
  {
    return abs(x) <= epsilon0;
  }

  inline real_t pow(const real_t x, const real_t y)
  {
    return ::powf(x, y);
  }

  inline real_t remndr(const real_t x, const real_t y)
  {
    return ::fmodf(x, y);
  }

  inline real_t sin(const real_t x)
  {
    return ::sinf(x);
  }

  inline real_t sinPI2(const signed int i)
  {
    static const real_t tab[4] = { 0, 1, 0, -1 };
    return tab[i & 0x3];
  }

  inline double sqrt(const double x)
  {
    return ::sqrt(x);
  }

  inline float sqrt(const float x)
  {
    return ::sqrtf(x);
  }

  inline real_t tan(const real_t x)
  {
    return ::tanf(x);
  }

} // namespace n4

#endif // N4_MATH_H
