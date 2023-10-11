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

#ifndef N4_COLOR3F_H
#define N4_COLOR3F_H

#include <cstdint>

#include <algorithm>
#include <limits>

#include <N4/Vector4f.h>

namespace n4 {

  ////// Type Traits /////////////////////////////////////////////////////////

  template<typename T>
  using is_rgb = std::bool_constant<std::is_integral_v<T>  &&  std::is_unsigned_v<T>  &&  sizeof(T) == 1>;

  template<typename T>
  using if_rgb_t = std::enable_if_t<is_rgb<T>::value,T>;

  ////// Types ///////////////////////////////////////////////////////////////

  using rgb_t = if_rgb_t<uint8_t>;

  ////// Color Property //////////////////////////////////////////////////////

  template<size_t index>
  class ColorProperty {
  public:
    ColorProperty(real_t *base) noexcept
      : _data(base + index)
    {
    }

    ~ColorProperty() noexcept = default;

    inline operator rgb_t() const
    {
      constexpr real_t MAX_RGB = static_cast<real_t>(256.0f - EPSILON0_PCT);
      return static_cast<rgb_t>(std::clamp<real_t>(*_data, 0, 1)*MAX_RGB);
    }

    inline rgb_t operator=(const rgb_t c)
    {
      constexpr real_t MAX_RGB_T = static_cast<real_t>(std::numeric_limits<rgb_t>::max());
      *_data = std::clamp<real_t>(static_cast<real_t>(c)/MAX_RGB_T, 0, 1);
      return c;
    }

  private:
    ColorProperty() noexcept = delete;

    real_t *_data{nullptr};
  };

  ////// Color Manipulator ///////////////////////////////////////////////////

  class Color3fManipulator {
  public:
    Color3fManipulator(real_t *data)
      : r(data)
      , g(data)
      , b(data)
      , r8(data)
      , g8(data)
      , b8(data)
    {
    }

    ~Color3fManipulator() noexcept = default;

    VectorProperty<0> r;
    VectorProperty<1> g;
    VectorProperty<2> b;

    ColorProperty<0> r8;
    ColorProperty<1> g8;
    ColorProperty<2> b8;

    inline real_t luminance() const
    {
      // NOTE 1: https://www.pbr-book.org/3ed-2018/Color_and_Radiometry/The_SampledSpectrum_Class
      // NOTE 2: https://github.com/boksajak/referencePT/blob/master/shaders/brdf.h
      constexpr real_t yr = 0.212671f;
      constexpr real_t yg = 0.715160f;
      constexpr real_t yb = 0.072169f;
      return yr*r + yg*g + yb*b;
    }

  private:
    Color3fManipulator() noexcept = delete;
  };

  ////// Color Traits ////////////////////////////////////////////////////////

  struct Color3fTraits {
    static constexpr bool have_w = false;
  };

  ////// Color Type //////////////////////////////////////////////////////////

  using Color3f = Vector4f<Color3fTraits,Color3fManipulator>;

} // namespace n4

#endif // N4_COLOR3F_H
