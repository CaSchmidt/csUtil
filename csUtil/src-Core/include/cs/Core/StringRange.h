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

#include <algorithm>
#include <iterator>

#include <cs/Core/CharUtil.h>

namespace cs {

  ////// Range Validation ////////////////////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  constexpr bool isValid(const T *first, const T *last)
  {
    return first != nullptr  &&  first < last;
  }

  template<typename T>
  requires IsCharacter<T>
  constexpr bool isValid(const T *str, const std::size_t len)
  {
    return str != nullptr  &&  len > 0;
  }

  ////// Range Length ////////////////////////////////////////////////////////

  template<typename T>
  requires IsCharacter<T>
  constexpr std::size_t distance(const T *first, const T *last)
  {
    return isValid(first, last)
        ? std::distance(first, last)
        : 0;
  }

  namespace impl_string {

    template<typename T>
    requires IsCharacter<T>
    constexpr std::size_t length2(const T *str)
    {
      return *str == glyph<T>::null
          ? 0
          : 1 + length2(str + 1);
    }

  } // namespace impl_string

  template<typename T>
  requires IsCharacter<T>
  constexpr std::size_t length(const T *str)
  {
    return str == nullptr
        ? 0
        : impl_string::length2(str);
  }

  template<typename T>
  requires IsCharacter<T>
  constexpr std::size_t length(const T *str, const std::size_t len)
  {
    return len == MAX_SIZE_T
        ? length(str)
        : len;
  }

  template<typename T>
  requires IsCharacter<T>
  inline std::size_t lengthRange(const T *first, const T *last)
  {
    return isValid(first, last)
        ? std::distance(first, std::find(first, last, glyph<T>::null))
        : 0;
  }

  template<typename T>
  requires IsCharacter<T>
  inline std::size_t lengthRange(const T *str, const std::size_t len)
  {
    return lengthRange(str, str + len);
  }

  template<typename T>
  requires IsCharacter<T>
  inline std::size_t lengthMax(const T *str, const std::size_t len)
  {
    return len == MAX_SIZE_T
        ? length(str)
        : lengthRange(str, len);
  }

} // namespace cs