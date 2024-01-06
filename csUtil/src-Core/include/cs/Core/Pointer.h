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

#include <cs/Core/TypeTraits.h>

namespace cs {

  struct Pointer {
    using value_type = typename IntegralOfSize<sizeof(void*)>::unsigned_type;

    template<typename T> requires is_pow2size_v<T>
    inline static bool isAlignedTo(const void *ptr)
    {
      constexpr value_type MASK = sizeof(T) - 1;
      return (to_value(ptr) & MASK) == ZERO;
    }

    template<typename T> requires is_pow2size_v<T>
    inline static void *alignTo(const void *ptr)
    {
      constexpr value_type MASK = sizeof(T) - 1;
      return to_pointer((to_value(ptr) + MASK) & ~MASK);
    }

    /*
     * NOTE:
     *
     * The purpose of the functions below is to check the validity of
     * range-based operations on fundamental types, usually applying
     * the standard algorithms.
     *
     * Checking the validity of a range of pointers can get tricky!
     *
     * 1. Arithmetic operations on pointers may overflow.
     *
     * 2. Validity may depend on the operating system's memory model;
     *    e.g. "Canonical Form Addresses":
     *    https://en.wikipedia.org/wiki/X86-64#Canonical_form_addresses
     *
     * The functions below validate the range [first, last) or
     * [ptr, ptr + siz), effectively excluding TOP.
     */

    inline static bool isValidRange(const void *first, const void *last)
    {
      return first != nullptr  &&  to_value(first) < to_value(last);
    }

    inline static bool isValidRange(const void *ptr, const std::size_t siz)
    {
      return ptr != nullptr  &&  siz > 0  &&  TOP - to_value(ptr) >= to_value(siz);
    }

  private:
    static constexpr value_type  TOP = std::numeric_limits<value_type>::max();
    static constexpr value_type ZERO = 0;

    inline static void *to_pointer(const value_type val)
    {
      return reinterpret_cast<void*>(val);
    }

    inline static value_type to_value(const std::size_t siz)
    {
      static_assert( sizeof(value_type) >= sizeof(std::size_t) );

      return static_cast<value_type>(siz);
    }

    inline static value_type to_value(const void *ptr)
    {
      return reinterpret_cast<value_type>(ptr);
    }
  };

} // namespace cs
