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

#include <cs/Math/Math.h>

namespace cs {

  template<typename T> requires IsReal<T>
  struct cmp {
    using value_type = T;

    static constexpr value_type DEFAULT_TOL = 0x1.0p-10;

    inline static bool eq(const value_type& a, const value_type& b,
                          const value_type& tol = DEFAULT_TOL)
    {
      return math<value_type>::abs(a - b) <= tol;
    }

    inline static bool geq(const value_type& a, const value_type& b,
                           const value_type& tol = DEFAULT_TOL)
    {
      return a - b >= -tol;
    }

    inline static bool gt(const value_type& a, const value_type& b,
                          const value_type& tol = DEFAULT_TOL)
    {
      return a - b > tol;
    }

    inline static bool leq(const value_type& a, const value_type& b,
                           const value_type& tol = DEFAULT_TOL)
    {
      return a - b <= tol;
    }

    inline static bool lt(const value_type& a, const value_type& b,
                          const value_type& tol = DEFAULT_TOL)
    {
      return a - b < -tol;
    }

    inline static bool neq(const value_type& a, const value_type& b,
                           const value_type& tol = DEFAULT_TOL)
    {
      return math<value_type>::abs(a - b) > tol;
    }
  };

} // namespace cs
