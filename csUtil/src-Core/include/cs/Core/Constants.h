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

#pragma once

#include <cs/Core/Concepts.h>

namespace cs {

  template<typename T> requires IsArithmetic<T>
  struct konst {
    using value_type = T;

    static constexpr auto MAX = std::numeric_limits<value_type>::max();
    static constexpr auto MIN = std::numeric_limits<value_type>::lowest();

    static constexpr value_type  ONE = 1;
    static constexpr value_type  TWO = 2;
    static constexpr value_type ZERO = 0;
  };

  /*
   * NOTE:
   * For the constants of e and pi cf. to "pi and e in Binary":
   * https://www.exploringbinary.com/pi-and-e-in-binary/
   */

  template<std::size_t SIZE>
  struct real_konst_impl {
    // SFINAE
  };

  template<>
  struct real_konst_impl<4> {
    using value_type = RealOfSize<4>::real_type;

    static constexpr auto INVALID_RESULT = std::numeric_limits<value_type>::quiet_NaN();

    static constexpr value_type  e = 0x1.5bf0a8p+1f;

    static constexpr value_type pi = 0x1.921fb6p+1f;

    static constexpr value_type milli = 0x1.0p-10f;

    static constexpr value_type micro = 0x1.0p-20f;
  };

  template<>
  struct real_konst_impl<8> {
    using value_type = RealOfSize<8>::real_type;

    static constexpr auto INVALID_RESULT = std::numeric_limits<value_type>::quiet_NaN();

    static constexpr value_type  e = 0x1.5bf0a8b145769p+1;

    static constexpr value_type pi = 0x1.921fb54442d18p+1;

    static constexpr value_type milli = 0x1.0p-10;

    static constexpr value_type micro = 0x1.0p-20;
  };

  template<typename T> requires IsReal<T>
  using real_konst = real_konst_impl<sizeof(T)>;

} // namespace cs
