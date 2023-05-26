/****************************************************************************
** Copyright (c) 2016, Carsten Schmidt. All rights reserved.
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

  struct Pointer {
    using value_type = typename IntegralOfSize<sizeof(void*)>::unsigned_type;

    template<typename T> requires IsPow2Size<T>
    inline static bool isAlignedTo(const void *ptr)
    {
      constexpr value_type MASK = sizeof(T) - 1;
      return (to_value(ptr) & MASK) == ZERO;
    }

    template<typename T> requires IsPow2Size<T>
    inline static void *alignTo(const void *ptr)
    {
      constexpr value_type MASK = sizeof(T) - 1;
      return to_pointer((to_value(ptr) + MASK) & ~MASK);
    }

  private:
    static constexpr value_type ZERO = 0;

    inline static void *to_pointer(const value_type val)
    {
      return reinterpret_cast<void*>(val);
    }

    inline static value_type to_value(const void *ptr)
    {
      return reinterpret_cast<value_type>(ptr);
    }
  };

} // namespace cs
