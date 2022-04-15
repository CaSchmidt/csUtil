/****************************************************************************
** Copyright (c) 2016, Carsten Schmidt. All rights reserved.
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

#ifndef CSPOINTER_H
#define CSPOINTER_H

#include <cstdint>

namespace cs {

  namespace impl_pointer {

    template<int Size>
    struct PointerTypeImpl {
      // SFINAE
    };

    template<>
    struct PointerTypeImpl<4> {
      using type = uint32_t;
    };

    template<>
    struct PointerTypeImpl<8> {
      using type = uint64_t;
    };

  } // namespace impl_pointer

} // namespace cs

struct csPointer {
  using type = typename cs::impl_pointer::PointerTypeImpl<sizeof(void*)>::type;

  template<typename DataT>
  inline static bool isAlignedTo(const void *p)
  {
    return ( reinterpret_cast<type>(p) & static_cast<type>(sizeof(DataT)-1) ) == 0;
  }

  template<typename DataT>
  inline static void *alignTo(const void *p)
  {
    return static_cast<void*>( reinterpret_cast<type>(p) & ~static_cast<type>(sizeof(DataT)-1) );
  }
};

#endif // CSPOINTER_H
