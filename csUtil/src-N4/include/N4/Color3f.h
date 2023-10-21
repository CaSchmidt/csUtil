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

#include <algorithm>

#include <N4/Vector4f.h>

namespace n4 {

  namespace impl_color3f {

    constexpr cs::byte_t to_c8(const real_t c)
    {
      constexpr real_t MAX = 255;
      constexpr real_t MIN = 0;
      return static_cast<cs::byte_t>(std::clamp(c*MAX, MIN, MAX));
    }

  } // namespace impl_color3f

  struct Color3fData {
    union {
      real_t _data[4];
      struct {
        real_t r, g, b;
      };
    };

    inline real_t luminance() const
    {
      // NOTE 1: https://www.pbr-book.org/3ed-2018/Color_and_Radiometry/The_SampledSpectrum_Class
      // NOTE 2: https://github.com/boksajak/referencePT/blob/master/shaders/brdf.h
      constexpr real_t yr = 0.212671f;
      constexpr real_t yg = 0.715160f;
      constexpr real_t yb = 0.072169f;
      return yr*r + yg*g + yb*b;
    }

    inline void pow(const real_t exponent)
    {
      using m = cs::Math<real_t>;
      r = m::pow(r, exponent);
      g = m::pow(g, exponent);
      b = m::pow(b, exponent);
    }

    inline cs::byte_t r8() const
    {
      return impl_color3f::to_c8(r);
    }

    inline cs::byte_t g8() const
    {
      return impl_color3f::to_c8(g);
    }

    inline cs::byte_t b8() const
    {
      return impl_color3f::to_c8(b);
    }
  };

  struct Color3fTraits {
    static constexpr bool have_w = false;
  };

  using Color3f = Vector4f<Color3fTraits,Color3fData>;

  inline Color3f pow(const Color3f& c, const real_t exponent)
  {
    Color3f result(c);
    result.pow(exponent);
    return result;
  }

} // namespace n4
