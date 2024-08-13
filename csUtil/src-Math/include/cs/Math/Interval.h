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

#include <cs/Core/Constants.h>

namespace cs {

  template<typename T> requires is_real_v<T>
  struct Interval {
    using value_type = T;

    // Assignment ////////////////////////////////////////////////////////////

    /*
     * Bounded interval: [begin, end]
     * Defaults to invalid null-interval.
     */
    Interval(const value_type begin = 0, const value_type end = 0) noexcept
      : begin{begin}
      , end{end}
    {
    }

    Interval(const Interval&) noexcept = default;
    Interval& operator=(const Interval&) noexcept = default;

    Interval(Interval&&) noexcept = default;
    Interval& operator=(Interval&&) noexcept = default;

    // Queries ///////////////////////////////////////////////////////////////

    inline bool isEmpty() const
    {
      return range() <= k::ZERO;
    }

    inline bool isNull() const
    {
      return range() == k::ZERO;
    }

    inline bool isValid() const
    {
      return range() > k::ZERO;
    }

    inline value_type center() const
    {
      return (begin + end)/k::TWO;
    }

    inline value_type radius() const
    {
      return (end - begin)/k::TWO;
    }

    /*
     * cf. https://en.wikipedia.org/wiki/Interval_(mathematics)#Definitions_and_terminology
     *
     * diameter, length, measure, range, size, width
     *
     * NOTE: range() may return negative values to indicate invalid intervals!
     */
    inline value_type range() const
    {
      return end - begin;
    }

    // Fixes /////////////////////////////////////////////////////////////////

    inline void adjust(const value_type range = Konst<value_type>::ONE)
    {
      if( isEmpty() ) {
        const value_type c = center();
        const value_type r = range/k::TWO;

        begin = c - r;
        end   = c + r;
      }
    }

    inline void normalize()
    {
      if( range() < k::ZERO ) {
        std::swap<value_type>(begin, end);
      }
    }

    // Subset ////////////////////////////////////////////////////////////////

    /*
     * NOTE:
     * The returned subset is selected by first normalizing 'other' using 'norm' and
     * then clamping the normalized interval to [0,1].
     */
    inline Interval subset(const Interval& other,
                           const value_type norm = Konst<value_type>::ONE)
    {
      if( !isValid()  ||  !other.isValid()  ||  norm <= k::ZERO ) {
        return Interval();
      }

      const value_type from = std::clamp<value_type>(other.begin/norm, 0, 1);
      const value_type   to = std::clamp<value_type>(other.end/norm, 0, 1);

      return Interval(begin + from*range(), begin + to*range());
    }

    // Update ////////////////////////////////////////////////////////////////

    inline void initializeUpdate()
    {
      begin = k::MAX;
      end   = k::MIN;
    }

    inline void update(const value_type x)
    {
      if( x < begin ) {
        begin = x;
      }
      if( x > end ) {
        end = x;
      }
    }

    inline bool update(const Interval& other)
    {
      // NOTE: this->isValid() always returns false if initializeUpdate() was called!
      if( !other.isValid() ) {
        return false;
      }
      if( other.begin < begin ) {
        begin = other.begin;
      }
      if( other.end > end ) {
        end = other.end;
      }
      return true;
    }

    // Data //////////////////////////////////////////////////////////////////

    value_type begin{};
    value_type end{};

    // Private ///////////////////////////////////////////////////////////////

  private:
    using k = Konst<value_type>;
  };

  // Intersection ////////////////////////////////////////////////////////////

  template<typename T, bool SANITY_CHECK = true>
  inline Interval<T> intersect(const Interval<T>& a, const Interval<T>& b)
  {
    if constexpr( SANITY_CHECK ) {
      if( !a.isValid()  ||  !b.isValid() ) {
        /*
         * NOTE:
         * Intentionally return an Interval<> with negative range() to prevent
         * intersects() to consider a default constructed Interval<> as
         * intersecting at tol == 0.
         */
        return Interval<T>(1, 0);
      }
    }

    const T begin = std::max(a.begin, b.begin);
    const T   end = std::min(a.end, b.end);

    return Interval<T>(begin, end); // return possibly empty Interval<>
  }

  template<typename T>
  inline Interval<T> operator&(const Interval<T>& a, const Interval<T>& b)
  {
    return intersect<T,true>(a, b);
  }

  template<typename T, bool SANITY_CHECK = true>
  inline bool intersects(const Interval<T>& a, const Interval<T>& b,
                         const T tol = 0)
  {
    const Interval<T> intv = intersect<T,SANITY_CHECK>(a, b);

    return intv.range() >= tol;
  }

  template<typename T>
  inline bool operator&&(const Interval<T>& a, const Interval<T>& b)
  {
    return intersects<T,true>(a, b, 0);
  }

  // Union ///////////////////////////////////////////////////////////////////

  template<typename T, bool SANITY_CHECK = true>
  inline Interval<T> unite(const Interval<T>& a, const Interval<T>& b)
  {
    if( !intersects<T,SANITY_CHECK>(a, b, 0) ) {
      return Interval<T>();
    }

    const T begin = std::min(a.begin, b.begin);
    const T   end = std::max(a.end, b.end);

    return Interval<T>(begin, end);
  }

  template<typename T>
  inline Interval<T> operator|(const Interval<T>& a, const Interval<T>& b)
  {
    return unite<T,true>(a, b);
  }

  template<typename T, bool SANITY_CHECK = true>
  inline Interval<T> uniteBounds(const Interval<T>& a, const Interval<T>& b)
  {
    if constexpr( SANITY_CHECK ) {
      if( !a.isValid()  ||  !b.isValid() ) {
        return Interval<T>();
      }
    }

    const T begin = std::min(a.begin, b.begin);
    const T   end = std::max(a.end, b.end);

    return Interval<T>(begin, end);
  }

  template<typename T>
  inline Interval<T> operator^(const Interval<T>& a, const Interval<T>& b)
  {
    return uniteBounds<T,true>(a, b);
  }

} // namespace cs
