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

#include <cs/Core/Constants.h>
#include <cs/Core/Pointer.h>
#include <cs/SIMD/impl/Reduce.h>

namespace cs {

  namespace simd {

    ////// Begin Implementation //////////////////////////////////////////////

    namespace impl_simd {

      // Generic Reduce Operations ///////////////////////////////////////////

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

      // Generic Reduce //////////////////////////////////////////////////////

      template<typename SIMD, typename OP, bool ALIGNED,
               typename  block_type = typename SIMD::block_type,
               typename result_type = ReduceResult<SIMD>,
               typename   size_type = typename SIMD::size_type,
               typename  value_type = typename SIMD::value_type>
      inline value_type reduce(const value_type *x, const size_type count)
      {
        if( x == nullptr  ||  count < 1 ) {
          return konst<value_type>::INVALID_RESULT;
        }

        const result_type result = reduce4Blocks<SIMD,OP,ALIGNED>(x, count);

        value_type sum = getSum<SIMD>(result);
        x += getReduced<SIMD>(result);

        const size_type numBlocks = getRemain<SIMD>(result)/SIMD::NUM_ELEMS;
        const size_type numRemain = getRemain<SIMD>(result)%SIMD::NUM_ELEMS;

        if( numBlocks > 0 ) {
          block_type acc = SIMD::zero();
          for(size_type i = 0; i < numBlocks; i++, x += SIMD::NUM_ELEMS) {
            const block_type block = SIMD::load<ALIGNED>(x);

            acc = SIMD::add(acc, OP::eval(block));
          }
          sum += SIMD::to_value(SIMD::hadd(acc));
        }

        for(size_type i = 0; i < numRemain; i++, x++) {
          sum += OP::eval(*x);
        }

        return sum;
      }

      template<typename SIMD, typename OP, bool ALIGNED_a, bool ALIGNED_b,
               typename  block_type = typename SIMD::block_type,
               typename result_type = ReduceResult<SIMD>,
               typename   size_type = typename SIMD::size_type,
               typename  value_type = typename SIMD::value_type>
      inline value_type reduce(const value_type *a, const value_type *b,
                               const size_type count)
      {
        if( a == nullptr  ||  b == nullptr  ||  count < 1 ) {
          return konst<value_type>::INVALID_RESULT;
        }

        const result_type result = reduce4Blocks<SIMD,OP,ALIGNED_a,ALIGNED_b>(a, b, count);

        value_type sum = getSum<SIMD>(result);
        a += getReduced<SIMD>(result);
        b += getReduced<SIMD>(result);

        const size_type numBlocks = getRemain<SIMD>(result)/SIMD::NUM_ELEMS;
        const size_type numRemain = getRemain<SIMD>(result)%SIMD::NUM_ELEMS;

        if( numBlocks > 0 ) {
          block_type acc = SIMD::zero();
          for(size_type i = 0; i < numBlocks; i++, a += SIMD::NUM_ELEMS, b += SIMD::NUM_ELEMS) {
            const block_type block_a = SIMD::load<ALIGNED_a>(a);
            const block_type block_b = SIMD::load<ALIGNED_b>(b);

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

    ////// End Implementation ////////////////////////////////////////////////

    ////// User Interface of SIMD Algorithms /////////////////////////////////

    template<typename SIMD>
    inline typename SIMD::value_type dot(const typename SIMD::value_type *a,
                                         const typename SIMD::value_type *b,
                                         const typename SIMD::size_type count)
    {
      using OP = impl_simd::OP_mul<SIMD>;
      const bool is_aligned_a = Pointer::isAlignedTo<typename SIMD::block_type>(a);
      const bool is_aligned_b = Pointer::isAlignedTo<typename SIMD::block_type>(b);
      if( is_aligned_a  &&  is_aligned_b ) {
        return impl_simd::reduce<SIMD,OP,true,true>(a, b, count);
      } else if( is_aligned_a ) {
        return impl_simd::reduce<SIMD,OP,true,false>(a, b, count);
      } else if( is_aligned_b ) {
        return impl_simd::reduce<SIMD,OP,false,true>(a, b, count);
      }
      return impl_simd::reduce<SIMD,OP,false,false>(a, b, count);
    }

    template<typename SIMD>
    inline typename SIMD::value_type sum(const typename SIMD::value_type *x,
                                         const typename SIMD::size_type count)
    {
      using OP = impl_simd::OP_identity<SIMD>;
      const bool is_aligned = Pointer::isAlignedTo<typename SIMD::block_type>(x);
      if( is_aligned ) {
        return impl_simd::reduce<SIMD,OP,true>(x, count);
      }
      return impl_simd::reduce<SIMD,OP,false>(x, count);
    }

    template<typename SIMD>
    inline typename SIMD::value_type sum_squared(const typename SIMD::value_type *x,
                                                 const typename SIMD::size_type count)
    {
      using OP = impl_simd::OP_square<SIMD>;
      const bool is_aligned = Pointer::isAlignedTo<typename SIMD::block_type>(x);
      if( is_aligned ) {
        return impl_simd::reduce<SIMD,OP,true>(x, count);
      }
      return impl_simd::reduce<SIMD,OP,false>(x, count);
    }

  } // namespace simd

} // namespace cs
