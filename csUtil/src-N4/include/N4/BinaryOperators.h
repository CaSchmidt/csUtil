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

#include <N4/Dispatch.h>
#include <N4/Matrix4f.h>

namespace n4 {

  ////// Implementation //////////////////////////////////////////////////////

  namespace impl_dispatch {

    struct BinAdd {
      inline static block_t eval(const block_t& a, const block_t& b)
      {
        return SIMD128::add(a, b);
      }
    };

    struct BinDiv {
      inline static block_t eval(const block_t& a, const block_t& b)
      {
        return SIMD128::div(a, b);
      }
    };

    struct BinMul {
      inline static block_t eval(const block_t& a, const block_t& b)
      {
        return SIMD128::mul(a, b);
      }
    };

    struct BinSub {
      inline static block_t eval(const block_t& a, const block_t& b)
      {
        return SIMD128::sub(a, b);
      }
    };

    template<typename traits_T, typename RHS>
    class BinTransform : public ExprBase<traits_T,BinTransform<traits_T,RHS>> {
    public:
      BinTransform(const Matrix4f& lhs, const RHS& rhs)
        : _lhs(lhs)
        , _rhs(rhs)
      {
      }

      inline block_t eval() const
      {
        using S = SIMD128;
        const block_t v = _rhs.eval();
        // NOTE: y = M*v
        block_t   y = S::mul(S::swizzle<0,0,0,0>(v), S::load(_lhs.data() + 0));
        y = S::add(y, S::mul(S::swizzle<1,1,1,1>(v), S::load(_lhs.data() + 4)));
        y = S::add(y, S::mul(S::swizzle<2,2,2,2>(v), S::load(_lhs.data() + 8)));
        if constexpr( traits_T::have_w ) {
          y = S::add(y, S::load(_lhs.data() + 12)); // NOTE: v.w := 1
        }
        return y;
      }

    private:
      const Matrix4f& _lhs;
      const RHS& _rhs;
    };

  } // namespace impl_dispatch

  ////// User Interface //////////////////////////////////////////////////////

  // Addition ////////////////////////////////////////////////////////////////

  template<typename traits_T, typename LHS, typename RHS>
  inline auto operator+(const ExprBase<traits_T,LHS>& lhs, const ExprBase<traits_T,RHS>& rhs)
  {
    using namespace impl_dispatch;
    return DispatchVV<BinAdd,traits_T,LHS,RHS>(lhs.as_derived(), rhs.as_derived());
  }

  // Subtraction /////////////////////////////////////////////////////////////

  template<typename traits_T, typename LHS, typename RHS>
  inline auto operator-(const ExprBase<traits_T,LHS>& lhs, const ExprBase<traits_T,RHS>& rhs)
  {
    using namespace impl_dispatch;
    return DispatchVV<BinSub,traits_T,LHS,RHS>(lhs.as_derived(), rhs.as_derived());
  }

  // Multiplication //////////////////////////////////////////////////////////

  template<typename traits_T, typename LHS, typename RHS>
  inline auto operator*(const ExprBase<traits_T,LHS>& lhs, const ExprBase<traits_T,RHS>& rhs)
  {
    using namespace impl_dispatch;
    return DispatchVV<BinMul,traits_T,LHS,RHS>(lhs.as_derived(), rhs.as_derived());
  }

  template<typename traits_T, typename LHS>
  inline auto operator*(const ExprBase<traits_T,LHS>& lhs, const real_t rhs)
  {
    using namespace impl_dispatch;
    return DispatchVS<BinMul,traits_T,LHS>(lhs.as_derived(), rhs);
  }

  template<typename traits_T, typename RHS>
  inline auto operator*(const real_t lhs, const ExprBase<traits_T,RHS>& rhs)
  {
    using namespace impl_dispatch;
    return DispatchSV<BinMul,traits_T,RHS>(lhs, rhs.as_derived());
  }

  template<typename traits_T, typename RHS>
  inline auto operator*(const Matrix4f& lhs, const ExprBase<traits_T,RHS>& rhs)
  {
    using namespace impl_dispatch;
    return BinTransform<traits_T,RHS>(lhs, rhs.as_derived());
  }

  // Division ////////////////////////////////////////////////////////////////

  template<typename traits_T, typename LHS, typename RHS>
  inline auto operator/(const ExprBase<traits_T,LHS>& lhs, const ExprBase<traits_T,RHS>& rhs)
  {
    using namespace impl_dispatch;
    return DispatchVV<BinDiv,traits_T,LHS,RHS>(lhs.as_derived(), rhs.as_derived());
  }

  template<typename traits_T, typename RHS>
  inline auto operator/(const real_t lhs, const ExprBase<traits_T,RHS>& rhs)
  {
    using namespace impl_dispatch;
    return DispatchSV<BinDiv,traits_T,RHS>(lhs, rhs.as_derived());
  }

  template<typename traits_T, typename LHS>
  inline auto operator/(const ExprBase<traits_T,LHS>& lhs, const real_t rhs)
  {
    using namespace impl_dispatch;
    return DispatchVS<BinDiv,traits_T,LHS>(lhs.as_derived(), rhs);
  }

} // namespace n4
