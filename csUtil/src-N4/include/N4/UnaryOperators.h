/****************************************************************************
** Copyright (c) 2020, Carsten Schmidt. All rights reserved.
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

#ifndef N4_UNARYOPERATORS_H
#define N4_UNARYOPERATORS_H

#include <N4/Dispatch.h>

namespace n4 {

  ////// Implementation //////////////////////////////////////////////////////

  namespace impl {

    struct UnaMinus {
      inline static simd_t eval(const simd_t& x)
      {
        return simd::sub(simd::zero(), x);
      }
    };

    struct UnaPlus {
      inline static simd_t eval(const simd_t& x)
      {
        return x;
      }
    };

  } // namespace impl

  ////// User Interface //////////////////////////////////////////////////////

  template<typename traits_T, typename OP>
  inline auto operator+(const ExprBase<traits_T,OP>& expr)
  {
    return impl::DispatchV<impl::UnaPlus,traits_T,OP>(expr.as_derived());
  }

  template<typename traits_T, typename OP>
  inline auto operator-(const ExprBase<traits_T,OP>& expr)
  {
    return impl::DispatchV<impl::UnaMinus,traits_T,OP>(expr.as_derived());
  }

} // namespace n4

#endif // N4_UNARYOPERATORS_H
