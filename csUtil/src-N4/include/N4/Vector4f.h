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
#include <cs/SIMD/SIMD128Vec4f.h>

#include <N4/ExprBase.h>
#include <N4/N4Traits.h>

namespace n4 {

  template<typename traits_T, typename data_T>
  requires is_vector4fdata_v<data_T>
  class alignas(SIMD128::block_type) Vector4f
      : public ExprBase<traits_T,Vector4f<traits_T,data_T>>
      , public data_T {
  public:
    ////// Assertions ////////////////////////////////////////////////////////

    static_assert( cs::simd::is_simd128x4f_v<SIMD128> );

    ////// Types /////////////////////////////////////////////////////////////

    using value_type = real_t;
    using typename ExprBase<traits_T,Vector4f<traits_T,data_T>>::traits_type;

    ////// Destructor ////////////////////////////////////////////////////////

    ~Vector4f() noexcept = default;

    ////// Constructor ///////////////////////////////////////////////////////

    Vector4f(const value_type val = 0) noexcept
    {
      set(val);
    }

    Vector4f& operator=(const value_type val)
    {
      set(val);
      return *this;
    }

    ////// Initialize ////////////////////////////////////////////////////////

    Vector4f(const std::initializer_list<value_type>& list) noexcept
    {
      initialize(list);
    }

    Vector4f& operator=(const std::initializer_list<value_type>& list)
    {
      initialize(list);
      return *this;
    }

    ////// Copy //////////////////////////////////////////////////////////////

    Vector4f(const Vector4f& other) noexcept
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
    {
      assign(expr);
    }

    template<typename EXPR>
    Vector4f& operator=(const ExprBase<traits_type,EXPR>& expr)
    {
      assign(expr);
      return *this;
    }

    inline block_t eval() const
    {
      return load();
    }

    ////// Assignment ////////////////////////////////////////////////////////

    template<typename EXPR>
    inline Vector4f& operator+=(const ExprBase<traits_type,EXPR>& expr)
    {
      store(S::add(load(), expr.as_derived().eval()));
      reset_w();
      return *this;
    }

    template<typename EXPR>
    inline Vector4f& operator-=(const ExprBase<traits_type,EXPR>& expr)
    {
      store(S::sub(load(), expr.as_derived().eval()));
      reset_w();
      return *this;
    }

    template<typename EXPR>
    inline Vector4f& operator*=(const ExprBase<traits_type,EXPR>& expr)
    {
      store(S::mul(load(), expr.as_derived().eval()));
      reset_w();
      return *this;
    }

    inline Vector4f& operator*=(const value_type val)
    {
      store(S::mul(load(), S::set(val)));
      reset_w();
      return *this;
    }

    template<typename EXPR>
    inline Vector4f& operator/=(const ExprBase<traits_type,EXPR>& expr)
    {
      store(S::div(load(), expr.as_derived().eval()));
      reset_w();
      return *this;
    }

    inline Vector4f& operator/=(const value_type val)
    {
      store(S::div(load(), S::set(val)));
      reset_w();
      return *this;
    }

    ////// Data Access ///////////////////////////////////////////////////////

    inline const value_type *data() const
    {
      return Vector4f::_data;
    }

    inline value_type *data()
    {
      return Vector4f::_data;
    }

    ////// Element Access ////////////////////////////////////////////////////

    constexpr size_t size() const
    {
      return 4;
    }

    inline value_type operator()(const size_t i) const
    {
      return Vector4f::_data[i];
    }

    inline value_type& operator()(const size_t i)
    {
      return Vector4f::_data[i];
    }

    ////// Functions /////////////////////////////////////////////////////////

    inline bool isNaN() const
    {
      return
          m::isNaN(Vector4f::_data[0])  ||
          m::isNaN(Vector4f::_data[1])  ||
          m::isNaN(Vector4f::_data[2]);
    }

    inline bool isZero(const value_type epsilon0 = EPSILON0_VECTOR) const
    {
      return cs::simd::isZero<S,false>(load(), epsilon0);
    }

    inline value_type max() const
    {
      const value_type x = std::max<value_type>(Vector4f::_data[0], Vector4f::_data[1]);
      return std::max<value_type>(x, Vector4f::_data[2]);
    }

    inline value_type min() const
    {
      const value_type x = std::min<value_type>(Vector4f::_data[0], Vector4f::_data[1]);
      return std::min<value_type>(x, Vector4f::_data[2]);
    }

  private:
    using m = cs::Math<value_type>;
    using S = SIMD128;

    static constexpr bool ALIGNED = true;

    template<typename EXPR>
    void assign(const ExprBase<traits_type,EXPR>& expr)
    {
      store(expr.as_derived().eval());
      if constexpr( !have_assign_w<EXPR>() ) {
        reset_w();
      }
    }

    inline void copy(const Vector4f& src)
    {
      store(src.load());
      reset_w();
    }

    inline void initialize(const std::initializer_list<value_type>& list)
    {
      if /*constexpr*/( list.size() > 0 ) {
        Vector4f::_data[0] = list.begin()[0];
      } else {
        Vector4f::_data[0] = 0;
      }
      if /*constexpr*/( list.size() > 1 ) {
        Vector4f::_data[1] = list.begin()[1];
      } else {
        Vector4f::_data[1] = 0;
      }
      if /*constexpr*/( list.size() > 2 ) {
        Vector4f::_data[2] = list.begin()[2];
      } else {
        Vector4f::_data[2] = 0;
      }
      reset_w();
    }

    inline block_t load() const
    {
      return S::load<ALIGNED>(Vector4f::_data);
    }

    inline void reset_w()
    {
      if constexpr( traits_type::have_w ) {
        Vector4f::_data[3] = 1;
      } else {
        Vector4f::_data[3] = 0;
      }
    }

    inline void set(const value_type val)
    {
      store(S::set(val));
      reset_w();
    }

    inline void store(const block_t& x)
    {
      S::store<ALIGNED>(Vector4f::_data, x);
    }
  };

} // namespace n4
