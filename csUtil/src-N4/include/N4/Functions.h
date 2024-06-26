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

#pragma once

#include <cs/SIMD/SIMD128Vec4f.h>

#include <N4/Dispatch.h>

namespace n4 {

  ////// Implementation //////////////////////////////////////////////////////

  namespace impl_dispatch {

    struct Abs {
      inline static block_t eval(const block_t& x)
      {
        return SIMD128::abs(x);
      }
    };

    struct Clamp {
      inline static block_t eval(const block_t& x, const block_t& lo, const block_t& hi)
      {
        return SIMD128::clamp(x, lo, hi);
      }
    };

    struct Cross {
      inline static block_t eval(const block_t& a, const block_t& b)
      {
        return cs::simd::cross<SIMD128>(a, b);
      }
    };

    struct Direction {
      inline static block_t eval(const block_t& from, const block_t& to)
      {
        const block_t x = SIMD128::sub(to, from);
        return SIMD128::div(x, SIMD128::sqrt(cs::simd::dot3<SIMD128>(x, x)));
      }
    };

    struct Max {
      inline static block_t eval(const block_t& a, const block_t& b)
      {
        return SIMD128::max(a, b);
      }
    };

    struct Min {
      inline static block_t eval(const block_t& a, const block_t& b)
      {
        return SIMD128::min(a, b);
      }
    };

    struct Normalize {
      inline static block_t eval(const block_t& x)
      {
        return SIMD128::div(x, SIMD128::sqrt(cs::simd::dot3<SIMD128>(x, x)));
      }
    };

  } // namespace impl_dispatch

  ////// User Interface //////////////////////////////////////////////////////

  template<typename traits_T, typename ARG>
  inline auto abs(const ExprBase<traits_T,ARG>& arg)
  {
    using namespace impl_dispatch;
    return DispatchV<Abs,traits_T,ARG>(arg.as_derived());
  }

  template<typename traits_T, typename ARG>
  inline auto clamp(const ExprBase<traits_T,ARG>& arg, const real_t lo, const real_t hi)
  {
    using namespace impl_dispatch;
    return DispatchVSS<Clamp,traits_T,ARG>(arg.as_derived(), lo, hi);
  }

  template<typename traits_T, typename ARG1, typename ARG2>
  inline auto cross(const ExprBase<traits_T,ARG1>& arg1, const ExprBase<traits_T,ARG2>& arg2)
  {
    using namespace impl_dispatch;
    return DispatchVV<Cross,traits_T,ARG1,ARG2>(arg1.as_derived(), arg2.as_derived());
  }

  template<typename traits_T, typename FROM, typename TO>
  inline auto direction(const ExprBase<traits_T,FROM>& from, const ExprBase<traits_T,TO>& to)
  {
    using namespace impl_dispatch;
    return DispatchVV<Direction,traits_T,FROM,TO>(from.as_derived(), to.as_derived());
  }

  template<typename traits_T, typename FROM, typename TO>
  inline real_t distance(const ExprBase<traits_T,FROM>& from, const ExprBase<traits_T,TO>& to)
  {
    const block_t delta = SIMD128::sub(to.as_derived().eval(), from.as_derived().eval());
    return SIMD128::to_value(SIMD128::sqrt(cs::simd::dot3<SIMD128>(delta, delta)));
  }

  template<typename traits_T, typename ARG1, typename ARG2>
  inline real_t dot(const ExprBase<traits_T,ARG1>& arg1, const ExprBase<traits_T,ARG2>& arg2)
  {
    return SIMD128::to_value(cs::simd::dot3<SIMD128>(arg1.as_derived().eval(), arg2.as_derived().eval()));
  }

  template<typename traits_T, typename ARG>
  inline real_t length(const ExprBase<traits_T,ARG>& arg)
  {
    const block_t x = arg.as_derived().eval();
    return SIMD128::to_value(SIMD128::sqrt(cs::simd::dot3<SIMD128>(x, x)));
  }

  template<typename traits_T, typename ARG1>
  inline auto max(const ExprBase<traits_T,ARG1>& arg1, const real_t arg2)
  {
    using namespace impl_dispatch;
    return DispatchVS<Max,traits_T,ARG1>(arg1.as_derived(), arg2);
  }

  template<typename traits_T, typename ARG2>
  inline auto max(const real_t arg1, const ExprBase<traits_T,ARG2>& arg2)
  {
    using namespace impl_dispatch;
    return DispatchSV<Max,traits_T,ARG2>(arg1, arg2.as_derived());
  }

  template<typename traits_T, typename ARG1, typename ARG2>
  inline auto max(const ExprBase<traits_T,ARG1>& arg1, const ExprBase<traits_T,ARG2>& arg2)
  {
    using namespace impl_dispatch;
    return DispatchVV<Max,traits_T,ARG1,ARG2>(arg1.as_derived(), arg2.as_derived());
  }

  template<typename traits_T, typename ARG1>
  inline auto min(const ExprBase<traits_T,ARG1>& arg1, const real_t arg2)
  {
    using namespace impl_dispatch;
    return DispatchVS<Min,traits_T,ARG1>(arg1.as_derived(), arg2);
  }

  template<typename traits_T, typename ARG2>
  inline auto min(const real_t arg1, const ExprBase<traits_T,ARG2>& arg2)
  {
    using namespace impl_dispatch;
    return DispatchSV<Min,traits_T,ARG2>(arg1, arg2.as_derived());
  }

  template<typename traits_T, typename ARG1, typename ARG2>
  inline auto min(const ExprBase<traits_T,ARG1>& arg1, const ExprBase<traits_T,ARG2>& arg2)
  {
    using namespace impl_dispatch;
    return DispatchVV<Min,traits_T,ARG1,ARG2>(arg1.as_derived(), arg2.as_derived());
  }

  template<typename traits_T, typename ARG>
  inline auto normalize(const ExprBase<traits_T,ARG>& arg)
  {
    using namespace impl_dispatch;
    return DispatchV<Normalize,traits_T,ARG>(arg.as_derived());
  }

} // namespace n4
