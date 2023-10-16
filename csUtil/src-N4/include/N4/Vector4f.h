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

#include <algorithm>
#include <initializer_list>

#include <cs/Math/Math.h>

#include <N4/ExprBase.h>
#include <N4/Manipulator.h>
#include <N4/SIMD.h>

namespace n4 {

  template<typename traits_T, typename manip_T = NoManipulator>
  class Vector4f
      : public ExprBase<traits_T,Vector4f<traits_T,manip_T>>
      , public manip_T {
  public:
    ////// Assertions ////////////////////////////////////////////////////////

    static_assert( cs::simd::is_simd128x4f_v<simd::SIMD128> );

    ////// Types /////////////////////////////////////////////////////////////

    using manip_type = manip_T;
    using value_type = real_t;
    using typename ExprBase<traits_T,Vector4f<traits_T,manip_T>>::traits_type;

    ////// Destructor ////////////////////////////////////////////////////////

    ~Vector4f() noexcept = default;

    ////// Constructor ///////////////////////////////////////////////////////

    Vector4f(const real_t val = 0) noexcept
      : manip_type(_data)
    {
      set(val);
    }

    Vector4f& operator=(const real_t val)
    {
      set(val);
      return *this;
    }

    ////// Initialize ////////////////////////////////////////////////////////

    Vector4f(const std::initializer_list<real_t>& list) noexcept
      : manip_type(_data)
    {
      initialize(list);
    }

    Vector4f& operator=(const std::initializer_list<real_t>& list)
    {
      initialize(list);
      return *this;
    }

    ////// Copy //////////////////////////////////////////////////////////////

    Vector4f(const Vector4f& other) noexcept
      : manip_type(_data)
    {
      copy(other);
    }

    Vector4f& operator=(const Vector4f& other)
    {
      copy(other);
      return *this;
    }

    ////// Move //////////////////////////////////////////////////////////////

    Vector4f(Vector4f&& other) noexcept
      : manip_type(_data)
    {
      copy(other);
    }

    Vector4f& operator=(Vector4f&& other)
    {
      copy(other);
      return *this;
    }

    ////// Expression ////////////////////////////////////////////////////////

    template<typename EXPR>
    Vector4f(const ExprBase<traits_type,EXPR>& expr) noexcept
      : manip_type(_data)
    {
      assign(expr);
    }

    template<typename EXPR>
    Vector4f& operator=(const ExprBase<traits_type,EXPR>& expr)
    {
      assign(expr);
      return *this;
    }

    inline simd::block_t eval() const
    {
      return S::load(_data);
    }

    ////// Assignment ////////////////////////////////////////////////////////

    template<typename EXPR>
    inline Vector4f& operator+=(const ExprBase<traits_type,EXPR>& expr)
    {
      S::store(_data, S::add(S::load(_data), expr.as_derived().eval()));
      reset_w();
      return *this;
    }

    template<typename EXPR>
    inline Vector4f& operator-=(const ExprBase<traits_type,EXPR>& expr)
    {
      S::store(_data, S::sub(S::load(_data), expr.as_derived().eval()));
      reset_w();
      return *this;
    }

    template<typename EXPR>
    inline Vector4f& operator*=(const ExprBase<traits_type,EXPR>& expr)
    {
      S::store(_data, S::mul(S::load(_data), expr.as_derived().eval()));
      reset_w();
      return *this;
    }

    inline Vector4f& operator*=(const real_t val)
    {
      S::store(_data, S::mul(S::load(_data), S::set(val)));
      reset_w();
      return *this;
    }

    template<typename EXPR>
    inline Vector4f& operator/=(const ExprBase<traits_type,EXPR>& expr)
    {
      S::store(_data, S::div(S::load(_data), expr.as_derived().eval()));
      reset_w();
      return *this;
    }

    inline Vector4f& operator/=(const real_t val)
    {
      S::store(_data, S::div(S::load(_data), S::set(val)));
      reset_w();
      return *this;
    }

    ////// Data Access ///////////////////////////////////////////////////////

    inline const real_t *data() const
    {
      return _data;
    }

    inline real_t *data()
    {
      return _data;
    }

    ////// Element Access ////////////////////////////////////////////////////

    constexpr size_t size() const
    {
      return 4;
    }

    inline real_t operator()(const size_t i) const
    {
      return _data[i];
    }

    inline real_t& operator()(const size_t i)
    {
      return _data[i];
    }

    ////// Functions /////////////////////////////////////////////////////////

    inline bool isNaN() const
    {
      return
          m::isNaN(_data[0])  ||
          m::isNaN(_data[1])  ||
          m::isNaN(_data[2]);
    }

    inline bool isZero(const real_t epsilon0 = EPSILON0_VECTOR) const
    {
      return simd::isZero<false>(S::load(_data), epsilon0);
    }

    inline real_t max() const
    {
      const real_t x = std::max<real_t>(_data[0], _data[1]);
      return std::max<real_t>(x, _data[2]);
    }

    inline real_t min() const
    {
      const real_t x = std::min<real_t>(_data[0], _data[1]);
      return std::min<real_t>(x, _data[2]);
    }

  private:
    using m = cs::Math<real_t>;
    using S = simd::SIMD128;

    template<typename EXPR>
    void assign(const ExprBase<traits_type,EXPR>& expr)
    {
      S::store(_data, expr.as_derived().eval());
      if constexpr( !have_assign_w<EXPR>() ) {
        reset_w();
      }
    }

    inline void copy(const Vector4f& src)
    {
      S::store(_data, S::load(src._data));
      reset_w();
    }

    inline void initialize(const std::initializer_list<real_t>& list)
    {
      if /*constexpr*/( list.size() > 0 ) {
        _data[0] = list.begin()[0];
      } else {
        _data[0] = 0;
      }
      if /*constexpr*/( list.size() > 1 ) {
        _data[1] = list.begin()[1];
      } else {
        _data[1] = 0;
      }
      if /*constexpr*/( list.size() > 2 ) {
        _data[2] = list.begin()[2];
      } else {
        _data[2] = 0;
      }
      reset_w();
    }

    inline void reset_w()
    {
      if constexpr( traits_type::have_w ) {
        _data[3] = 1;
      } else {
        _data[3] = 0;
      }
    }

    inline void set(const real_t val)
    {
      S::store(_data, S::set(val));
      reset_w();
    }

  private:
    alignas(sizeof(S::block_type)) real_t _data[4];
  };

} // namespace n4
