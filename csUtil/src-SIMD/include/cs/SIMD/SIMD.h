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

#include <cs/Core/Pointer.h>
#include <cs/SIMD/impl/Reduce.h>
#include <cs/SIMD/OP.h>

namespace cs {

  namespace simd {

    ////// User Interface of SIMD Algorithms /////////////////////////////////

    template<typename SIMD, template<typename> typename OP>
    inline typename SIMD::value_type reduce(const typename SIMD::value_type *x,
                                            const typename SIMD::size_type count)
    {
      const bool is_aligned = cs::Pointer::isAlignedTo<typename SIMD::block_type>(x);
      if( is_aligned ) {
        return impl_simd::reduce<SIMD,OP<SIMD>,true>(x, count);
      }
      return impl_simd::reduce<SIMD,OP<SIMD>,false>(x, count);
    }

    template<typename SIMD, template<typename> typename OP>
    inline typename SIMD::value_type reduce(const typename SIMD::value_type *a,
                                            const typename SIMD::value_type *b,
                                            const typename SIMD::size_type count)
    {
      const bool is_aligned_a = cs::Pointer::isAlignedTo<typename SIMD::block_type>(a);
      const bool is_aligned_b = cs::Pointer::isAlignedTo<typename SIMD::block_type>(b);
      if( is_aligned_a  &&  is_aligned_b ) {
        return impl_simd::reduce<SIMD,OP<SIMD>,true,true>(a, b, count);
      } else if( is_aligned_a ) {
        return impl_simd::reduce<SIMD,OP<SIMD>,true,false>(a, b, count);
      } else if( is_aligned_b ) {
        return impl_simd::reduce<SIMD,OP<SIMD>,false,true>(a, b, count);
      }
      return impl_simd::reduce<SIMD,OP<SIMD>,false,false>(a, b, count);
    }

    template<typename SIMD>
    inline typename SIMD::value_type dot(const typename SIMD::value_type *a,
                                         const typename SIMD::value_type *b,
                                         const typename SIMD::size_type count)
    {
      return reduce<SIMD,OP_mul>(a, b, count);
    }

    template<typename SIMD>
    inline typename SIMD::value_type sum(const typename SIMD::value_type *x,
                                         const typename SIMD::size_type count)
    {
      return reduce<SIMD,OP_identity>(x, count);
    }

    template<typename SIMD>
    inline typename SIMD::value_type sum_squared(const typename SIMD::value_type *x,
                                                 const typename SIMD::size_type count)
    {
      return reduce<SIMD,OP_square>(x, count);
    }

  } // namespace simd

} // namespace cs
