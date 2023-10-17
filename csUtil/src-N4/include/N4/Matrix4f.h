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

#include <N4/N4Traits.h>
#include <N4/SIMD.h>

namespace n4 {

  ////// 4x4 Column-Major Matrix - Implementation ////////////////////////////

  class Matrix4f {
  public:
    ////// Destructor ////////////////////////////////////////////////////////

    ~Matrix4f() noexcept = default;

    ////// Constructor ///////////////////////////////////////////////////////

    Matrix4f(const real_t val = 0) noexcept
    {
      set(val);
    }

    Matrix4f& operator=(const real_t val)
    {
      set(val);
      return *this;
    }

    ////// Initialize (Row-Major List!) //////////////////////////////////////

    Matrix4f(const std::initializer_list<real_t>& list) noexcept
    {
      initialize(list);
    }

    Matrix4f& operator=(const std::initializer_list<real_t>& list)
    {
      initialize(list);
      return *this;
    }

    ////// Copy //////////////////////////////////////////////////////////////

    Matrix4f(const Matrix4f& other) noexcept
    {
      copy(other);
    }

    Matrix4f& operator=(const Matrix4f& other)
    {
      copy(other);
      return *this;
    }

    ////// Move //////////////////////////////////////////////////////////////

    Matrix4f(Matrix4f&& other) noexcept
    {
      copy(other);
    }

    Matrix4f& operator=(Matrix4f&& other)
    {
      copy(other);
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
      return 16;
    }

    constexpr size_t rows() const
    {
      return 4;
    }

    constexpr size_t columns() const
    {
      return 4;
    }

    inline real_t operator()(const size_t i, const size_t j) const
    {
      return _data[i + j*4]; // Column-Major!
    }

    inline real_t& operator()(const size_t i, const size_t j)
    {
      return _data[i + j*4]; // Column-Major!
    }

    ////// Functions /////////////////////////////////////////////////////////

    inline Matrix4f inverse() const
    {
      Matrix4f result;
      cs::simd::inverse<S>(result._data, _data);
      return result;
    }

    inline bool isZero(const real_t epsilon0 = EPSILON0_VECTOR) const
    {
      const simd::block_t _epsilon0 = S::set(epsilon0);
      return
          cs::simd::isZero<S>(S::load(_data +  0), _epsilon0)  &&
          cs::simd::isZero<S>(S::load(_data +  4), _epsilon0)  &&
          cs::simd::isZero<S>(S::load(_data +  8), _epsilon0)  &&
          cs::simd::isZero<S>(S::load(_data + 12), _epsilon0);
    }

    inline Matrix4f transpose() const
    {
      Matrix4f result;
      cs::simd::transpose<S>(result._data, _data);
      return result;
    }

  private:
    using S = simd::SIMD128;

    inline void copy(const Matrix4f& other)
    {
      S::store(_data +  0, S::load(other._data +  0));
      S::store(_data +  4, S::load(other._data +  4));
      S::store(_data +  8, S::load(other._data +  8));
      S::store(_data + 12, S::load(other._data + 12));
    }

    inline void initialize(const std::initializer_list<real_t>& list)
    {
      set(0);
      const size_t s = std::min<size_t>(list.size(), size());
      for(size_t l = 0; l < s; l++) {
        // NOTE: Row-Major Notation: l = i*4 + j
        const size_t i = l/4;
        const size_t j = l%4;
        operator()(i, j) = list.begin()[l];
      }
    }

    inline void set(const real_t val)
    {
      const simd::block_t col = S::set(val);
      S::store(_data +  0, col);
      S::store(_data +  4, col);
      S::store(_data +  8, col);
      S::store(_data + 12, col);
    }

    alignas(sizeof(S::block_type)) real_t _data[16];
  };

  ////// 4x4 Matrix - Binary Operators ///////////////////////////////////////

  inline Matrix4f operator*(const Matrix4f& lhs, const Matrix4f& rhs)
  {
    using S = simd::SIMD128;
    using namespace simd;
    Matrix4f M;
    const block_t col0 = S::load(lhs.data() +  0);
    const block_t col1 = S::load(lhs.data() +  4);
    const block_t col2 = S::load(lhs.data() +  8);
    const block_t col3 = S::load(lhs.data() + 12);
    S::store(M.data() +  0, cs::simd::transform<S>(col0, col1, col2, col3, S::load(rhs.data() +  0)));
    S::store(M.data() +  4, cs::simd::transform<S>(col0, col1, col2, col3, S::load(rhs.data() +  4)));
    S::store(M.data() +  8, cs::simd::transform<S>(col0, col1, col2, col3, S::load(rhs.data() +  8)));
    S::store(M.data() + 12, cs::simd::transform<S>(col0, col1, col2, col3, S::load(rhs.data() + 12)));
    return M;
  }

  ////// 4x4 Matrix - Functions //////////////////////////////////////////////

  namespace impl {

    inline Matrix4f rotateX(const real_t c, const real_t s)
    {
      return Matrix4f{
        1, 0, 0, 0,
        0, c,-s, 0,
        0, s, c, 0,
        0, 0, 0, 1
      };
    }

    inline Matrix4f rotateY(const real_t c, const real_t s)
    {
      return Matrix4f{
        c, 0, s, 0,
        0, 1, 0, 0,
       -s, 0, c, 0,
        0, 0, 0, 1
      };
    }

    inline Matrix4f rotateZ(const real_t c, const real_t s)
    {
      return Matrix4f{
        c,-s, 0, 0,
        s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
      };
    }

  } // namespace impl

  inline Matrix4f identity()
  {
    return Matrix4f{
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
    };
  }

  inline Matrix4f rotateX(const real_t radians)
  {
    using m = cs::Math<real_t>;
    return impl::rotateX(m::cos(radians), m::sin(radians));
  }

  inline Matrix4f rotateXbyPI2(const signed int i)
  {
    using m = cs::Math<real_t>;
    return impl::rotateX(m::cosPI2(i), m::sinPI2(i));
  }

  inline Matrix4f rotateY(const real_t radians)
  {
    using m = cs::Math<real_t>;
    return impl::rotateY(m::cos(radians), m::sin(radians));
  }

  inline Matrix4f rotateYbyPI2(const signed int i)
  {
    using m = cs::Math<real_t>;
    return impl::rotateY(m::cosPI2(i), m::sinPI2(i));
  }

  inline Matrix4f rotateZ(const real_t radians)
  {
    using m = cs::Math<real_t>;
    return impl::rotateZ(m::cos(radians), m::sin(radians));
  }

  inline Matrix4f rotateZbyPI2(const signed int i)
  {
    using m = cs::Math<real_t>;
    return impl::rotateZ(m::cosPI2(i), m::sinPI2(i));
  }

  inline Matrix4f scale(const real_t sx, const real_t sy, const real_t sz)
  {
    return Matrix4f{
      sx,  0,  0, 0,
       0, sy,  0, 0,
       0,  0, sz, 0,
       0,  0,  0, 1
    };
  }

  inline Matrix4f translate(const real_t tx, const real_t ty, const real_t tz)
  {
    return Matrix4f{
      1, 0, 0, tx,
      0, 1, 0, ty,
      0, 0, 1, tz,
      0, 0, 0,  1
    };
  }

} // namespace n4
