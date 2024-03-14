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

#include <cs/Core/Bit.h>

namespace cs {

  namespace saturate {

    /************************************************************************
     * Implementation *******************************************************
     ************************************************************************/

    namespace impl_sat {

      template<typename T>
      constexpr make_unsigned_t<T> abs(const T x)
      {
        return x >= Konst<T>::ZERO
            ? unsigned_cast(x)
            : unsigned_cast((~x) + Konst<T>::ONE);
      }

      template<typename T>
      inline if_unsigned_t<T> mul(const T& a, const T& b, const T& max)
      {
        // (1) Sanity Check & Optimization ///////////////////////////////////

        if( a == 0  ||  b == 0  ||  max < 1 ) {
          return 0;
        }

        const T lhs = std::min(a, b);
        const T rhs = std::max(a, b);

        const int numRhsBits = std::popcount(rhs);

        // (2) Loop over LHS' Binary Digits //////////////////////////////////

        T result = 0;

        for(std::size_t i = NUM_BITS<T>; i > 0; i--) {
          const std::size_t bit = i - 1;

          // (2.1) Skip Zero Digit ///////////////////////////////////////////

          if( !testBit(lhs, bit) ) {
            continue;
          }

          // (2.2) Does summand overflow? ////////////////////////////////////

          const T summand = rhs << bit;
          if( summand >= max  ||  std::popcount(summand) < numRhsBits ) {
            return max;
          }

          // (2.3) Does sum overflow? ////////////////////////////////////////

          const T sum = result + summand;
          if( sum >= max  ||  sum < result ) {
            return max;
          }

          // (2.4) Update Result /////////////////////////////////////////////

          result = sum;
        } // for( i )

        return result;
      }

      template<typename T>
      constexpr make_signed_t<T> neg(const T x)
      {
        return signed_cast((~x) + Konst<T>::ONE);
      }

    } // namespace impl_sat

    /************************************************************************
     * Signed ***************************************************************
     ************************************************************************/

    template<typename T>
    constexpr if_signed_t<T> add(const T& a, const T& b)
    {
      const T result = a + b;
      if(        testBit(~a & ~b &  result, MAX_BIT<T>) ) { // pos -> neg
        return Konst<T>::MAX;
      } else if( testBit( a &  b & ~result, MAX_BIT<T>) ) { // neg -> pos
        return Konst<T>::MIN;
      }
      return result;
    }

    template<typename T>
    constexpr if_signed_t<T> add_bl(const T& a, const T& b)
    {
      const T _result = a + b;
      T results[2] = { _result, Konst<T>::MAX };
      results[0] = results[testBit(~a & ~b &  _result, MAX_BIT<T>)]; // pos -> neg
      results[1] = Konst<T>::MIN;
      return results[testBit( a &  b & ~_result, MAX_BIT<T>)]; // neg -> pos
    }

    template<typename T>
    inline if_signed_t<T> mul(const T& a, const T& b)
    {
      using UnsignedT = make_unsigned_t<T>;

      const bool is_neg_result = testBit(a ^ b, MAX_BIT<T>);

      const UnsignedT max = is_neg_result
          ? Konst<T>::MIN
          : Konst<T>::MAX;

      const UnsignedT result = impl_sat::mul(impl_sat::abs(a), impl_sat::abs(b), max);

      return is_neg_result
          ? impl_sat::neg(result)
          : result;
    }

    template<typename T>
    constexpr if_signed_t<T> sub(const T& a, const T& b)
    {
      const T result = a - b;
      if(        testBit(~a &  b &  result, MAX_BIT<T>) ) { // pos -> neg
        return Konst<T>::MAX;
      } else if( testBit( a & ~b & ~result, MAX_BIT<T>) ) { // neg -> pos
        return Konst<T>::MIN;
      }
      return result;
    }

    template<typename T>
    constexpr if_signed_t<T> sub_bl(const T& a, const T& b)
    {
      const T _result = a - b;
      T results[2] = { _result, Konst<T>::MAX };
      results[0] = results[testBit(~a &  b &  _result, MAX_BIT<T>)]; // pos -> neg
      results[1] = Konst<T>::MIN;
      return results[testBit( a & ~b & ~_result, MAX_BIT<T>)]; // neg -> pos
    }

    /************************************************************************
     * Unsigned *************************************************************
     ************************************************************************/

    template<typename T>
    constexpr if_unsigned_t<T> add(const T& a, const T& b)
    {
      const T result = a + b;
      if( result < a ) { // MAX -> 0
        return Konst<T>::MAX;
      }
      return result;
    }

    template<typename T>
    constexpr if_unsigned_t<T> add_bl(const T& a, const T& b)
    {
      const T _result = a + b;
      const T results[2] = { _result, Konst<T>::MAX };
      return results[_result < a]; // MAX -> 0
    }

    template<typename T>
    inline if_unsigned_t<T> mul(const T& a, const T& b)
    {
      return impl_sat::mul<T>(a, b, Konst<T>::MAX);
    }

    template<typename T>
    constexpr if_unsigned_t<T> sub(const T& a, const T& b)
    {
      const T result = a - b;
      if( result > a ) { // 0 -> MAX
        return Konst<T>::MIN;
      }
      return result;
    }

    template<typename T>
    constexpr if_unsigned_t<T> sub_bl(const T& a, const T& b)
    {
      const T _result = a - b;
      const T results[2] = { _result, Konst<T>::MIN };
      return results[_result > a]; // 0 -> MAX
    }

    /************************************************************************
     * Arbitrary Signedness *************************************************
     ************************************************************************/

    template<typename T>
    constexpr if_integral_t<T> dec(const T& x)
    {
      const T result = x - 1;
      if( x == Konst<T>::MIN ) {
        return Konst<T>::MIN;
      }
      return result;
    }

    template<typename T>
    constexpr if_integral_t<T> dec_bl(const T& x)
    {
      const T _result = x - 1;
      const T results[2] = { _result, Konst<T>::MIN };
      return results[x == Konst<T>::MIN];
    }

    template<typename T>
    constexpr if_integral_t<T> inc(const T& x)
    {
      const T result = x + 1;
      if( x == Konst<T>::MAX ) {
        return Konst<T>::MAX;
      }
      return result;
    }

    template<typename T>
    constexpr if_integral_t<T> inc_bl(const T& x)
    {
      const T _result = x + 1;
      const T results[2] = { _result, Konst<T>::MAX };
      return results[x == Konst<T>::MAX];
    }

  } // namespace saturate

} // namespace cs
