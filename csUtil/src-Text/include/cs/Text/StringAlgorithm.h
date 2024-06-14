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

#include <cs/Core/CharUtil.h>
#include <cs/Core/Range.h>

namespace cs {

  ////// Skip leading whitespace /////////////////////////////////////////////

  namespace impl_string {

    template<typename T> requires is_char_v<T>
    inline const T *skipSpace(const T *first, const T *last)
    {
      return std::find_if_not(first, last, lambda_is_space<T>());
    }

  } // namespace impl_string

  template<typename T> requires is_char_v<T>
  inline const T *skipSpace_s(const T *str, const std::size_t len)
  {
    const std::size_t max = strlen(str, len);

    return max > 0
        ? impl_string::skipSpace(str, str + max)
        : nullptr;
  }

  template<typename T> requires is_char_v<T>
  inline const T *skipSpace(const T *str, const std::size_t len)
  {
    return impl_string::skipSpace(str, str + len);
  }

  template<typename T> requires is_char_v<T>
  inline const T *skipSpace(const T *str)
  {
    const std::size_t max = strlen(str);

    return max > 0
        ? impl_string::skipSpace(str, str + max)
        : nullptr;
  }

} // namespace cs
