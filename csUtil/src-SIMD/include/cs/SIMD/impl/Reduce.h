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

namespace cs {

  namespace simd {

    namespace impl_simd {

      template<typename SIMD,
               typename  size_type = typename SIMD::size_type,
               typename value_type = typename SIMD::value_type>
      using ReduceResult = std::tuple<value_type,size_type,size_type>;

      template<typename SIMD,
               typename value_type = typename SIMD::value_type>
      constexpr value_type getSum(const ReduceResult<SIMD>& result)
      {
        return std::get<0>(result);
      }

      template<typename SIMD,
               typename size_type = typename SIMD::size_type>
      constexpr size_type getReduced(const ReduceResult<SIMD>& result)
      {
        return std::get<1>(result);
      }

      template<typename SIMD,
               typename size_type = typename SIMD::size_type>
      constexpr size_type getRemain(const ReduceResult<SIMD>& result)
      {
        return std::get<2>(result);
      }

      template<typename SIMD, typename OP, bool ALIGNED,
               typename  block_type = typename SIMD::block_type,
               typename result_type = ReduceResult<SIMD>,
               typename   size_type = typename SIMD::size_type,
               typename  value_type = typename SIMD::value_type>
      inline result_type reduce4Blocks(const value_type *x, const size_type count)
      {
        constexpr size_type REDUCE_STRIDE = SIMD::NUM_ELEMS*4;

        const size_type numReduce      = count/REDUCE_STRIDE;
        const size_type numElemsRemain = count%REDUCE_STRIDE;

        if( numReduce < 1 ) {
          return result_type{value_type{0}, size_type{0}, count};
        }

        block_type acc = SIMD::zero();
        for(size_type i = 0; i < numReduce; i++, x += REDUCE_STRIDE) {
          SIMD::prefetch(x + SIMD::NUM_ELEMS*4);

          const block_type block1 = SIMD::load<ALIGNED>(x + SIMD::NUM_ELEMS*0);
          const block_type block2 = SIMD::load<ALIGNED>(x + SIMD::NUM_ELEMS*1);
          const block_type block3 = SIMD::load<ALIGNED>(x + SIMD::NUM_ELEMS*2);
          const block_type block4 = SIMD::load<ALIGNED>(x + SIMD::NUM_ELEMS*3);

          acc = SIMD::add(acc, OP::eval(block1));
          acc = SIMD::add(acc, OP::eval(block2));
          acc = SIMD::add(acc, OP::eval(block3));
          acc = SIMD::add(acc, OP::eval(block4));
        }
        const value_type sum = SIMD::to_value(SIMD::hadd(acc));

        return result_type{sum, numReduce*REDUCE_STRIDE, numElemsRemain};
      }

      template<typename SIMD, typename OP, bool ALIGNED_a, bool ALIGNED_b,
               typename  block_type = typename SIMD::block_type,
               typename result_type = ReduceResult<SIMD>,
               typename   size_type = typename SIMD::size_type,
               typename  value_type = typename SIMD::value_type>
      inline result_type reduce4Blocks(const value_type *a, const value_type *b,
                                       const size_type count)
      {
        constexpr size_type REDUCE_STRIDE = SIMD::NUM_ELEMS*4;

        const size_type numReduce      = count/REDUCE_STRIDE;
        const size_type numElemsRemain = count%REDUCE_STRIDE;

        if( numReduce < 1 ) {
          return result_type{value_type{0}, size_type{0}, count};
        }

        block_type acc = SIMD::zero();
        for(size_type i = 0; i < numReduce; i++, a += REDUCE_STRIDE, b += REDUCE_STRIDE) {
          SIMD::prefetch(a + SIMD::NUM_ELEMS*4);
          SIMD::prefetch(b + SIMD::NUM_ELEMS*4);

          const block_type block_a1 = SIMD::load<ALIGNED_a>(a + SIMD::NUM_ELEMS*0);
          const block_type block_b1 = SIMD::load<ALIGNED_b>(b + SIMD::NUM_ELEMS*0);

          const block_type block_a2 = SIMD::load<ALIGNED_a>(a + SIMD::NUM_ELEMS*1);
          const block_type block_b2 = SIMD::load<ALIGNED_b>(b + SIMD::NUM_ELEMS*1);

          const block_type block_a3 = SIMD::load<ALIGNED_a>(a + SIMD::NUM_ELEMS*2);
          const block_type block_b3 = SIMD::load<ALIGNED_b>(b + SIMD::NUM_ELEMS*2);

          const block_type block_a4 = SIMD::load<ALIGNED_a>(a + SIMD::NUM_ELEMS*3);
          const block_type block_b4 = SIMD::load<ALIGNED_b>(b + SIMD::NUM_ELEMS*3);

          acc = SIMD::add(acc, OP::eval(block_a1, block_b1));
          acc = SIMD::add(acc, OP::eval(block_a2, block_b2));
          acc = SIMD::add(acc, OP::eval(block_a3, block_b3));
          acc = SIMD::add(acc, OP::eval(block_a4, block_b4));
        }
        const value_type sum = SIMD::to_value(SIMD::hadd(acc));

        return result_type{sum, numReduce*REDUCE_STRIDE, numElemsRemain};
      }

    } // namespace impl_simd

  } // namespace simd

} // namespace cs
