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

#include <math.h>

#include <cs/Core/Constants.h>

namespace cs {

  template<std::size_t SIZE>
  struct MathImpl {
    // SFINAE
  };

  template<>
  struct MathImpl<4> {
    using value_type = RealOfSize<4>::real_type;

    inline static value_type abs(const value_type& x)
    {
      return ::fabsf(x);
    }

    inline static value_type acos(const value_type& x)
    {
      return ::acosf(x);
    }

    inline static value_type atan2(const value_type& y, const value_type& x)
    {
      return ::atan2f(y, x);
    }

    inline static value_type ceil(const value_type& x)
    {
      return ::ceilf(x);
    }

    inline static value_type cos(const value_type& x)
    {
      return ::cosf(x);
    }

    inline static value_type cosPI2(const signed int i)
    {
      static const value_type tab[4] = { 1, 0, -1, 0 };
      return tab[i & 0x3];
    }

    inline static value_type floor(const value_type& x)
    {
      return ::floorf(x);
    }

    inline static value_type fract(const value_type& x)
    {
      value_type i;
      return ::modff(x, &i);
    }

    inline static value_type intgr(const value_type& x)
    {
      value_type i;
      ::modff(x, &i);
      return i;
    }

    inline static bool isFinite(const value_type& x)
    {
      return isfinite(x) != 0;
    }

    inline static bool isInfinite(const value_type& x)
    {
      return isinf(x) != 0;
    }

    inline static bool isNaN(const value_type& x)
    {
      return isnan(x) != 0;
    }

    inline static bool isZero(const value_type& x,
                              const value_type& tol = RealKonst<value_type>::micro)
    {
      return abs(x) <= tol;
    }

    inline static value_type log(const value_type& x)
    {
      return ::logf(x);
    }

    inline static value_type log10(const value_type& x)
    {
      return ::log10f(x);
    }

    inline static value_type mod(const value_type& x, const value_type& y)
    {
      return ::fmodf(x, y);
    }

    inline static value_type pow(const value_type& base, const value_type& exponent)
    {
      return ::powf(base, exponent);
    }

    inline static value_type round(const value_type& x)
    {
      return ::roundf(x);
    }

    inline static value_type sin(const value_type& x)
    {
      return ::sinf(x);
    }

    inline static value_type sinPI2(const signed int i)
    {
      static const value_type tab[4] = { 0, 1, 0, -1 };
      return tab[i & 0x3];
    }

    inline static value_type sqrt(const value_type& x)
    {
      return ::sqrtf(x);
    }

    inline static value_type tan(const value_type& x)
    {
      return ::tanf(x);
    }
  };

  template<>
  struct MathImpl<8> {
    using value_type = RealOfSize<8>::real_type;

    inline static value_type abs(const value_type& x)
    {
      return ::fabs(x);
    }

    inline static value_type acos(const value_type& x)
    {
      return ::acos(x);
    }

    inline static value_type atan2(const value_type& y, const value_type& x)
    {
      return ::atan2(y, x);
    }

    inline static value_type ceil(const value_type& x)
    {
      return ::ceil(x);
    }

    inline static value_type cos(const value_type& x)
    {
      return ::cos(x);
    }

    inline static value_type cosPI2(const signed int i)
    {
      static const value_type tab[4] = { 1, 0, -1, 0 };
      return tab[i & 0x3];
    }

    inline static value_type floor(const value_type& x)
    {
      return ::floor(x);
    }

    inline static value_type fract(const value_type& x)
    {
      value_type i;
      return ::modf(x, &i);
    }

    inline static value_type intgr(const value_type& x)
    {
      value_type i;
      ::modf(x, &i);
      return i;
    }

    inline static bool isFinite(const value_type& x)
    {
      return isfinite(x) != 0;
    }

    inline static bool isInfinite(const value_type& x)
    {
      return isinf(x) != 0;
    }

    inline static bool isNaN(const value_type& x)
    {
      return isnan(x) != 0;
    }

    inline static bool isZero(const value_type& x,
                              const value_type& tol = RealKonst<value_type>::micro)
    {
      return abs(x) <= tol;
    }

    inline static value_type log(const value_type& x)
    {
      return ::log(x);
    }

    inline static value_type log10(const value_type& x)
    {
      return ::log10(x);
    }

    inline static value_type mod(const value_type& x, const value_type& y)
    {
      return ::fmod(x, y);
    }

    inline static value_type pow(const value_type& base, const value_type& exponent)
    {
      return ::pow(base, exponent);
    }

    inline static value_type round(const value_type& x)
    {
      return ::round(x);
    }

    inline static value_type sin(const value_type& x)
    {
      return ::sin(x);
    }

    inline static value_type sinPI2(const signed int i)
    {
      static const value_type tab[4] = { 0, 1, 0, -1 };
      return tab[i & 0x3];
    }

    inline static value_type sqrt(const value_type& x)
    {
      return ::sqrt(x);
    }

    inline static value_type tan(const value_type& x)
    {
      return ::tan(x);
    }
  };

  template<typename T> requires is_real_v<T>
  using Math = MathImpl<sizeof(T)>;

} // namespace cs
