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

#include <cs/Core/Concepts.h>

namespace cs {

  template<std::size_t SIZE>
  struct math_impl {
    // SFINAE
  };

  template<>
  struct math_impl<4> {
    using value_type = RealOfSize<4>::real_type;

    inline static value_type abs(const value_type& x)
    {
      return ::fabsf(x);
    }

    inline static value_type ceil(const value_type& x)
    {
      return ::ceilf(x);
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

    inline static value_type log(const value_type& x)
    {
      return ::logf(x);
    }

    inline static value_type log10(const value_type& x)
    {
      return ::log10f(x);
    }

    inline static value_type pow(const value_type& base, const value_type& exponent)
    {
      return ::powf(base, exponent);
    }

    inline static value_type round(const value_type& x)
    {
      return ::roundf(x);
    }

    inline static value_type sqrt(const value_type& x)
    {
      return ::sqrtf(x);
    }
  };

  template<>
  struct math_impl<8> {
    using value_type = RealOfSize<8>::real_type;

    inline static value_type abs(const value_type& x)
    {
      return ::fabs(x);
    }

    inline static value_type ceil(const value_type& x)
    {
      return ::ceil(x);
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

    inline static value_type log(const value_type& x)
    {
      return ::log(x);
    }

    inline static value_type log10(const value_type& x)
    {
      return ::log10(x);
    }

    inline static value_type pow(const value_type& base, const value_type& exponent)
    {
      return ::pow(base, exponent);
    }

    inline static value_type round(const value_type& x)
    {
      return ::round(x);
    }

    inline static value_type sqrt(const value_type& x)
    {
      return ::sqrt(x);
    }
  };

  template<typename T> requires IsReal<T>
  using math = math_impl<sizeof(T)>;

} // namespace cs
