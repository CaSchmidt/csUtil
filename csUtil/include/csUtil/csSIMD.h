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

    namespace impl_simd {

      template<typename SIMD>
      struct OP_identity {
        inline static typename SIMD::real_type eval(const typename SIMD::real_type& x)
        {
          return x;
        }

        inline static typename SIMD::simd_type eval(const typename SIMD::simd_type& x)
        {
          return x;
        }
      };

      template<typename SIMD>
      struct OP_mul {
        inline static typename SIMD::real_type eval(const typename SIMD::real_type& a,
                                                    const typename SIMD::real_type& b)
        {
          return a*b;
        }

        inline static typename SIMD::simd_type eval(const typename SIMD::simd_type& a,
                                                    const typename SIMD::simd_type& b)
        {
          return SIMD::mul(a, b);
        }
      };

      template<typename SIMD>
      struct OP_square {
        inline static typename SIMD::real_type eval(const typename SIMD::real_type& x)
        {
          return x*x;
        }

        inline static typename SIMD::simd_type eval(const typename SIMD::simd_type& x)
        {
          return SIMD::mul(x, x);
        }
      };

      template<typename SIMD, typename OP, bool ALIGNED>
      inline typename SIMD::real_type accumulate(const typename SIMD::real_type *x,
                                                 const typename SIMD::size_type count)
      {
        using real_type = typename SIMD::real_type;
        using simd_type = typename SIMD::simd_type;
        using size_type = typename SIMD::size_type;

        if( x == nullptr  ||  count < 1 ) {
          return std::numeric_limits<real_type>::quiet_NaN();
        }

        const size_type numBlocks = count/SIMD::NUM_ELEMS;
        const size_type numRemain = count%SIMD::NUM_ELEMS;

        real_type sum = 0;

        if( numBlocks > 0 ) {
          simd_type acc = SIMD::zero();
          for(size_type i = 0; i < numBlocks; i++, x += SIMD::NUM_ELEMS) {
            const simd_type block = ALIGNED
                ? SIMD::load(x)
                : SIMD::load_unaligned(x);
            acc = SIMD::add(acc, OP::eval(block));
          }
          sum += SIMD::to_real(SIMD::hadd(acc));
        }

        for(size_type i = 0; i < numRemain; i++, x++) {
          sum += OP::eval(*x);
        }

        return sum;
      }

      template<typename SIMD, typename OP, bool ALIGNED_a, bool ALIGNED_b>
      inline typename SIMD::real_type accumulate(const typename SIMD::real_type *a,
                                                 const typename SIMD::real_type *b,
                                                 const typename SIMD::size_type count)
      {
        using real_type = typename SIMD::real_type;
        using simd_type = typename SIMD::simd_type;
        using size_type = typename SIMD::size_type;

        if( a == nullptr  ||  b == nullptr  ||  count < 1 ) {
          return std::numeric_limits<real_type>::quiet_NaN();
        }

        const size_type numBlocks = count/SIMD::NUM_ELEMS;
        const size_type numRemain = count%SIMD::NUM_ELEMS;

        real_type sum = 0;

        if( numBlocks > 0 ) {
          simd_type acc = SIMD::zero();
          for(size_type i = 0; i < numBlocks; i++, a += SIMD::NUM_ELEMS, b += SIMD::NUM_ELEMS) {
            const simd_type block_a = ALIGNED_a
                ? SIMD::load(a)
                : SIMD::load_unaligned(a);
            const simd_type block_b = ALIGNED_b
                ? SIMD::load(b)
                : SIMD::load_unaligned(b);
            acc = SIMD::add(acc, OP::eval(block_a, block_b));
          }
          sum += SIMD::to_real(SIMD::hadd(acc));
        }

        for(size_type i = 0; i < numRemain; i++, a++, b++) {
          sum += OP::eval(*a, *b);
        }

        return sum;
      }

    } // namespace impl_simd

    template<typename SIMD>
    inline typename SIMD::real_type dot(const typename SIMD::real_type *a,
                                        const typename SIMD::real_type *b,
                                        const typename SIMD::size_type count)
    {
      using OP = impl_simd::OP_mul<SIMD>;
      const bool is_aligned_a = csPointer::isAlignedTo<typename SIMD::simd_type>(a);
      const bool is_aligned_b = csPointer::isAlignedTo<typename SIMD::simd_type>(b);
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
    inline typename SIMD::real_type sum(const typename SIMD::real_type *x,
                                        const typename SIMD::size_type count)
    {
      using OP = impl_simd::OP_identity<SIMD>;
      const bool is_aligned = csPointer::isAlignedTo<typename SIMD::simd_type>(x);
      return is_aligned
          ? impl_simd::accumulate<SIMD,OP,true>(x, count)
          : impl_simd::accumulate<SIMD,OP,false>(x, count);
    }

    template<typename SIMD>
    inline typename SIMD::real_type sum_squared(const typename SIMD::real_type *x,
                                                const typename SIMD::size_type count)
    {
      using OP = impl_simd::OP_square<SIMD>;
      const bool is_aligned = csPointer::isAlignedTo<typename SIMD::simd_type>(x);
      return is_aligned
          ? impl_simd::accumulate<SIMD,OP,true>(x, count)
          : impl_simd::accumulate<SIMD,OP,false>(x, count);
    }

  } // namespace simd

} // namespace cs

#endif // CSSIMD_H
