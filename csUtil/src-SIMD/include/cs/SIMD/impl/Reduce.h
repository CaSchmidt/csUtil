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

#include <tuple>

#include <cs/Core/Constants.h>

namespace cs {

  namespace simd {

    namespace impl_simd {

      // Result of Reduction /////////////////////////////////////////////////

      template<typename SIMD>
      using ReduceResult = std::tuple<
      typename SIMD::value_type, // result: sum of reduced elements
      typename SIMD::size_type,  // number of reduced elements
      typename SIMD::size_type   // number of remaining (AKA not reduced) elements
      >;

      template<typename SIMD>
      constexpr typename SIMD::value_type getSum(const ReduceResult<SIMD>& result)
      {
        return std::get<0>(result);
      }

      template<typename SIMD>
      constexpr typename SIMD::size_type getReduced(const ReduceResult<SIMD>& result)
      {
        return std::get<1>(result);
      }

      template<typename SIMD>
      constexpr typename SIMD::size_type getRemain(const ReduceResult<SIMD>& result)
      {
        return std::get<2>(result);
      }

      // Reduction of Single Vector //////////////////////////////////////////

      template<typename SIMD, typename OP, bool ALIGNED>
      inline ReduceResult<SIMD> reduce4Blocks(const typename SIMD::value_type *x,
                                              const typename SIMD::size_type count)
      {
        using  block_type = typename SIMD::block_type;
        using result_type = ReduceResult<SIMD>;
        using   size_type = typename SIMD::size_type;
        using  value_type = typename SIMD::value_type;

        constexpr size_type REDUCE_STRIDE = SIMD::NUM_ELEM*4;

        const size_type numReduce      = count/REDUCE_STRIDE;
        const size_type numElemsRemain = count%REDUCE_STRIDE;

        if( numReduce < 1 ) {
          return result_type{value_type{0}, size_type{0}, count};
        }

        block_type acc = SIMD::zero();
        for(size_type i = 0; i < numReduce; i++, x += REDUCE_STRIDE) {
          SIMD::prefetch(x + REDUCE_STRIDE);

          acc = SIMD::add(acc, OP::eval(SIMD::template load<ALIGNED>(x + SIMD::NUM_ELEM*0)));
          acc = SIMD::add(acc, OP::eval(SIMD::template load<ALIGNED>(x + SIMD::NUM_ELEM*1)));
          acc = SIMD::add(acc, OP::eval(SIMD::template load<ALIGNED>(x + SIMD::NUM_ELEM*2)));
          acc = SIMD::add(acc, OP::eval(SIMD::template load<ALIGNED>(x + SIMD::NUM_ELEM*3)));
        }
        const value_type sum = SIMD::to_value(SIMD::hadd(acc));

        return result_type{sum, numReduce*REDUCE_STRIDE, numElemsRemain};
      }

      template<typename SIMD, typename OP, bool ALIGNED>
      inline typename SIMD::value_type reduce(const typename SIMD::value_type *x,
                                              const typename SIMD::size_type count)
      {
        using  block_type = typename SIMD::block_type;
        using result_type = ReduceResult<SIMD>;
        using   size_type = typename SIMD::size_type;
        using  value_type = typename SIMD::value_type;

        if( x == nullptr  ||  count < 1 ) {
          return konst<value_type>::INVALID_RESULT;
        }

        const result_type result = reduce4Blocks<SIMD,OP,ALIGNED>(x, count);

        value_type sum = getSum<SIMD>(result);
        x += getReduced<SIMD>(result);

        const size_type numBlocks = getRemain<SIMD>(result)/SIMD::NUM_ELEM;
        const size_type numRemain = getRemain<SIMD>(result)%SIMD::NUM_ELEM;

        if( numBlocks > 0 ) {
          block_type acc = SIMD::zero();
          for(size_type i = 0; i < numBlocks; i++, x += SIMD::NUM_ELEM) {
            const block_type block = SIMD::template load<ALIGNED>(x);

            acc = SIMD::add(acc, OP::eval(block));
          }
          sum += SIMD::to_value(SIMD::hadd(acc));
        }

        for(size_type i = 0; i < numRemain; i++, x++) {
          sum += OP::eval(*x);
        }

        return sum;
      }

      // Reduction of Two Vectors ////////////////////////////////////////////

      template<typename SIMD, typename OP, bool ALIGNED_a, bool ALIGNED_b>
      inline ReduceResult<SIMD> reduce4Blocks(const typename SIMD::value_type *a,
                                              const typename SIMD::value_type *b,
                                              const typename SIMD::size_type count)
      {
        using  block_type = typename SIMD::block_type;
        using result_type = ReduceResult<SIMD>;
        using   size_type = typename SIMD::size_type;
        using  value_type = typename SIMD::value_type;

        constexpr size_type REDUCE_STRIDE = SIMD::NUM_ELEM*4;

        const size_type numReduce      = count/REDUCE_STRIDE;
        const size_type numElemsRemain = count%REDUCE_STRIDE;

        if( numReduce < 1 ) {
          return result_type{value_type{0}, size_type{0}, count};
        }

        block_type acc = SIMD::zero();
        for(size_type i = 0; i < numReduce; i++, a += REDUCE_STRIDE, b += REDUCE_STRIDE) {
          SIMD::prefetch(a + REDUCE_STRIDE);
          SIMD::prefetch(b + REDUCE_STRIDE);

          acc = SIMD::add(acc, OP::eval(SIMD::template load<ALIGNED_a>(a + SIMD::NUM_ELEM*0),
                                        SIMD::template load<ALIGNED_b>(b + SIMD::NUM_ELEM*0)));
          acc = SIMD::add(acc, OP::eval(SIMD::template load<ALIGNED_a>(a + SIMD::NUM_ELEM*1),
                                        SIMD::template load<ALIGNED_b>(b + SIMD::NUM_ELEM*1)));
          acc = SIMD::add(acc, OP::eval(SIMD::template load<ALIGNED_a>(a + SIMD::NUM_ELEM*2),
                                        SIMD::template load<ALIGNED_b>(b + SIMD::NUM_ELEM*2)));
          acc = SIMD::add(acc, OP::eval(SIMD::template load<ALIGNED_a>(a + SIMD::NUM_ELEM*3),
                                        SIMD::template load<ALIGNED_b>(b + SIMD::NUM_ELEM*3)));
        }
        const value_type sum = SIMD::to_value(SIMD::hadd(acc));

        return result_type{sum, numReduce*REDUCE_STRIDE, numElemsRemain};
      }

      template<typename SIMD, typename OP, bool ALIGNED_a, bool ALIGNED_b>
      inline typename SIMD::value_type reduce(const typename SIMD::value_type *a,
                                              const typename SIMD::value_type *b,
                                              const typename SIMD::size_type count)
      {
        using  block_type = typename SIMD::block_type;
        using result_type = ReduceResult<SIMD>;
        using   size_type = typename SIMD::size_type;
        using  value_type = typename SIMD::value_type;

        if( a == nullptr  ||  b == nullptr  ||  count < 1 ) {
          return konst<value_type>::INVALID_RESULT;
        }

        const result_type result = reduce4Blocks<SIMD,OP,ALIGNED_a,ALIGNED_b>(a, b, count);

        value_type sum = getSum<SIMD>(result);
        a += getReduced<SIMD>(result);
        b += getReduced<SIMD>(result);

        const size_type numBlocks = getRemain<SIMD>(result)/SIMD::NUM_ELEM;
        const size_type numRemain = getRemain<SIMD>(result)%SIMD::NUM_ELEM;

        if( numBlocks > 0 ) {
          block_type acc = SIMD::zero();
          for(size_type i = 0; i < numBlocks; i++, a += SIMD::NUM_ELEM, b += SIMD::NUM_ELEM) {
            const block_type block_a = SIMD::template load<ALIGNED_a>(a);
            const block_type block_b = SIMD::template load<ALIGNED_b>(b);

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

  } // namespace simd

} // namespace cs
