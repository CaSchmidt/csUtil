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

#ifndef CSSIMD_H
#define CSSIMD_H

#include <limits>

#include <csUtil/csPointer.h>

namespace cs {

  namespace simd {

    template<typename SIMD, bool ALIGNED>
    inline typename SIMD::block_type load(const typename SIMD::value_type *ptr)
    {
      if constexpr( ALIGNED ) {
        return SIMD::load(ptr);
      }
      return SIMD::load_unaligned(ptr);
    }

    namespace impl_simd {

      // Generic Operations //////////////////////////////////////////////////

      template<typename SIMD>
      struct OP_identity {
        inline static typename SIMD::value_type eval(const typename SIMD::value_type& x)
        {
          return x;
        }

        inline static typename SIMD::block_type eval(const typename SIMD::block_type& x)
        {
          return x;
        }
      };

      template<typename SIMD>
      struct OP_mul {
        inline static typename SIMD::value_type eval(const typename SIMD::value_type& a,
                                                     const typename SIMD::value_type& b)
        {
          return a*b;
        }

        inline static typename SIMD::block_type eval(const typename SIMD::block_type& a,
                                                     const typename SIMD::block_type& b)
        {
          return SIMD::mul(a, b);
        }
      };

      template<typename SIMD>
      struct OP_square {
        inline static typename SIMD::value_type eval(const typename SIMD::value_type& x)
        {
          return x*x;
        }

        inline static typename SIMD::block_type eval(const typename SIMD::block_type& x)
        {
          return SIMD::mul(x, x);
        }
      };

      // Generic Accumulator //////////////////////////////////////////////////

#ifdef HAVE_SIMD128_PREFETCH
      inline constexpr std::size_t SIZE_CACHE_LINE = 64; // Assumed size of one cache line in bytes.
#endif

      template<typename SIMD, typename OP, bool ALIGNED>
      inline typename SIMD::value_type accumulate(const typename SIMD::value_type *x,
                                                  const typename SIMD::size_type count)
      {
        using block_type = typename SIMD::block_type;
        using  size_type = typename SIMD::size_type;
        using value_type = typename SIMD::value_type;

        if( x == nullptr  ||  count < 1 ) {
          return std::numeric_limits<value_type>::quiet_NaN();
        }

        value_type sum = 0;

#ifdef HAVE_SIMD128_PREFETCH // Move to if constexpr( ... )?
        static_assert( SIZE_CACHE_LINE/sizeof(block_type) == 4 );

        constexpr size_type NUM_ELEMS_LINE = SIMD::NUM_ELEMS*4;

        const size_type numLines       = count/NUM_ELEMS_LINE;
        const size_type numLinesRemain = count%NUM_ELEMS_LINE;

        if( numLines > 0 ) {
          block_type acc = SIMD::zero();
          for(size_type i = 0; i < numLines; i++) {
            SIMD::prefetch(x + NUM_ELEMS_LINE);

            const block_type block1 = load<SIMD,ALIGNED>(x);
            x += SIMD::NUM_ELEMS;
            const block_type block2 = load<SIMD,ALIGNED>(x);
            x += SIMD::NUM_ELEMS;
            const block_type block3 = load<SIMD,ALIGNED>(x);
            x += SIMD::NUM_ELEMS;
            const block_type block4 = load<SIMD,ALIGNED>(x);
            x += SIMD::NUM_ELEMS;

            acc = SIMD::add(acc, OP::eval(block1));
            acc = SIMD::add(acc, OP::eval(block2));
            acc = SIMD::add(acc, OP::eval(block3));
            acc = SIMD::add(acc, OP::eval(block4));
          }
          sum += SIMD::to_value(SIMD::hadd(acc));
        }

        const size_type numBlocks = numLinesRemain/SIMD::NUM_ELEMS;
        const size_type numRemain = numLinesRemain%SIMD::NUM_ELEMS;
#else
        const size_type numBlocks = count/SIMD::NUM_ELEMS;
        const size_type numRemain = count%SIMD::NUM_ELEMS;
#endif

        if( numBlocks > 0 ) {
          block_type acc = SIMD::zero();
          for(size_type i = 0; i < numBlocks; i++) {
            const block_type block = load<SIMD,ALIGNED>(x);
            x += SIMD::NUM_ELEMS;

            acc = SIMD::add(acc, OP::eval(block));
          }
          sum += SIMD::to_value(SIMD::hadd(acc));
        }

        for(size_type i = 0; i < numRemain; i++, x++) {
          sum += OP::eval(*x);
        }

        return sum;
      }

      template<typename SIMD, typename OP, bool ALIGNED_a, bool ALIGNED_b>
      inline typename SIMD::value_type accumulate(const typename SIMD::value_type *a,
                                                  const typename SIMD::value_type *b,
                                                  const typename SIMD::size_type count)
      {
        using block_type = typename SIMD::block_type;
        using  size_type = typename SIMD::size_type;
        using value_type = typename SIMD::value_type;

        if( a == nullptr  ||  b == nullptr  ||  count < 1 ) {
          return std::numeric_limits<value_type>::quiet_NaN();
        }

        value_type sum = 0;

#ifdef HAVE_SIMD128_PREFETCH // Move to if constexpr( ... )?
        static_assert( SIZE_CACHE_LINE/sizeof(block_type) == 4 );

        constexpr size_type NUM_ELEMS_LINE = SIMD::NUM_ELEMS*4;

        const size_type numLines       = count/NUM_ELEMS_LINE;
        const size_type numLinesRemain = count%NUM_ELEMS_LINE;

        if( numLines > 0 ) {
          block_type acc = SIMD::zero();
          for(size_type i = 0; i < numLines; i++) {
            SIMD::prefetch(a + NUM_ELEMS_LINE);
            SIMD::prefetch(b + NUM_ELEMS_LINE);

            const block_type block_a1 = load<SIMD,ALIGNED_a>(a);
            a += SIMD::NUM_ELEMS;
            const block_type block_a2 = load<SIMD,ALIGNED_a>(a);
            a += SIMD::NUM_ELEMS;
            const block_type block_b1 = load<SIMD,ALIGNED_b>(b);
            b += SIMD::NUM_ELEMS;
            const block_type block_b2 = load<SIMD,ALIGNED_b>(b);
            b += SIMD::NUM_ELEMS;

            acc = SIMD::add(acc, OP::eval(block_a1, block_b1));
            acc = SIMD::add(acc, OP::eval(block_a2, block_b2));

            const block_type block_a3 = load<SIMD,ALIGNED_a>(a);
            a += SIMD::NUM_ELEMS;
            const block_type block_a4 = load<SIMD,ALIGNED_a>(a);
            a += SIMD::NUM_ELEMS;
            const block_type block_b3 = load<SIMD,ALIGNED_b>(b);
            b += SIMD::NUM_ELEMS;
            const block_type block_b4 = load<SIMD,ALIGNED_b>(b);
            b += SIMD::NUM_ELEMS;

            acc = SIMD::add(acc, OP::eval(block_a3, block_b3));
            acc = SIMD::add(acc, OP::eval(block_a4, block_b4));
          }
          sum += SIMD::to_value(SIMD::hadd(acc));
        }

        const size_type numBlocks = numLinesRemain/SIMD::NUM_ELEMS;
        const size_type numRemain = numLinesRemain%SIMD::NUM_ELEMS;
#else
        const size_type numBlocks = count/SIMD::NUM_ELEMS;
        const size_type numRemain = count%SIMD::NUM_ELEMS;
#endif

        if( numBlocks > 0 ) {
          block_type acc = SIMD::zero();
          for(size_type i = 0; i < numBlocks; i++) {
            const block_type block_a = load<SIMD,ALIGNED_a>(a);
            a += SIMD::NUM_ELEMS;
            const block_type block_b = load<SIMD,ALIGNED_b>(b);
            b += SIMD::NUM_ELEMS;

            acc = SIMD::add(acc, OP::eval(block_a, block_b));
          }
          sum += SIMD::to_value(SIMD::hadd(acc));
        }

        for(size_type i = 0; i < numRemain; i++, a++, b++) {
          sum += OP::eval(*a, *b);
        }

        return sum;
      }

    } // namespace impl_simd

    template<typename SIMD>
    inline typename SIMD::value_type dot(const typename SIMD::value_type *a,
                                         const typename SIMD::value_type *b,
                                         const typename SIMD::size_type count)
    {
      using OP = impl_simd::OP_mul<SIMD>;
      const bool is_aligned_a = csPointer::isAlignedTo<typename SIMD::block_type>(a);
      const bool is_aligned_b = csPointer::isAlignedTo<typename SIMD::block_type>(b);
      if( is_aligned_a  &&  is_aligned_b ) {
        return impl_simd::accumulate<SIMD,OP,true,true>(a, b, count);
      } else if( is_aligned_a ) {
        return impl_simd::accumulate<SIMD,OP,true,false>(a, b, count);
      } else if( is_aligned_b ) {
        return impl_simd::accumulate<SIMD,OP,false,true>(a, b, count);
      }
      return impl_simd::accumulate<SIMD,OP,false,false>(a, b, count);
    }

    template<typename SIMD>
    inline typename SIMD::value_type sum(const typename SIMD::value_type *x,
                                         const typename SIMD::size_type count)
    {
      using OP = impl_simd::OP_identity<SIMD>;
      const bool is_aligned = csPointer::isAlignedTo<typename SIMD::block_type>(x);
      if( is_aligned ) {
        return impl_simd::accumulate<SIMD,OP,true>(x, count);
      }
      return impl_simd::accumulate<SIMD,OP,false>(x, count);
    }

    template<typename SIMD>
    inline typename SIMD::value_type sum_squared(const typename SIMD::value_type *x,
                                                 const typename SIMD::size_type count)
    {
      using OP = impl_simd::OP_square<SIMD>;
      const bool is_aligned = csPointer::isAlignedTo<typename SIMD::block_type>(x);
      if( is_aligned ) {
        return impl_simd::accumulate<SIMD,OP,true>(x, count);
      }
      return impl_simd::accumulate<SIMD,OP,false>(x, count);
    }

  } // namespace simd

} // namespace cs

#endif // CSSIMD_H
