/****************************************************************************
** Copyright (c) 2023, Carsten Schmidt. All rights reserved.
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

#include <cs/Core/CharUtil.h>
#include <cs/Core/Pointer.h>

namespace cs {

  ////// Implementation //////////////////////////////////////////////////////

  namespace impl_range {

    template<typename T>
    requires IsArithmetic<T>  ||  IsCharacter<T>
    inline std::size_t size(const T *first, const T *last)
    {
      static_assert( sizeof(std::size_t) >= sizeof(std::ptrdiff_t) );

      const std::ptrdiff_t diff = last - first;
      return diff > 0
          ? diff
          : 0;
    }

    template<typename T>
    requires IsCharacter<T>
    inline std::size_t strlen(const T *first, const T *last)
    {
      return size(first, std::ranges::find(first, last, glyph<T>::null));
    }

  } // namespace impl_range

  ////// Public //////////////////////////////////////////////////////////////

  template<typename T>
  requires IsArithmetic<T>  ||  IsCharacter<T>
  inline std::size_t size(const T *first, const T *last)
  {
    return Pointer::isValidRange(first, last)
        ? impl_range::size(first, last)
        : 0;
  }

  template<typename T>
  requires IsArithmetic<T>  ||  IsCharacter<T>
  inline std::size_t size(const T *ptr, const std::size_t siz)
  {
    return Pointer::isValidRange(ptr, siz)
        ? siz
        : 0;
  }

  template<typename T> requires IsCharacter<T>
  inline std::size_t strlen(const T *first, const T *last)
  {
    return Pointer::isValidRange(first, last)
        ? impl_range::strlen(first, last)
        : 0;
  }

  template<typename T> requires IsCharacter<T>
  inline std::size_t strlen(const T *str, const std::size_t siz = MAX_SIZE_T)
  {
    return siz == MAX_SIZE_T
        ? strlen(str, Pointer::top<decltype(str)>())
        : Pointer::isValidRange(str, siz)
          ? impl_range::strlen(str, str + siz)
          : 0;
  }

} // namespace cs
