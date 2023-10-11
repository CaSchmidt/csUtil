/****************************************************************************
** Copyright (c) 2021, Carsten Schmidt. All rights reserved.
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

#ifndef N4_UTIL_H
#define N4_UTIL_H

#include <N4/ExprCast.h>
#include <N4/Functions.h>
#include <N4/Matrix4f.h>

namespace n4 {

  namespace util {

    namespace impl {

      template<typename VecT>
      inline size_t absMinAt(const VecT& v)
      {
        size_t at = n4::abs(v(0)) <= n4::abs(v(1))
            ? 0
            : 1;
        if( n4::abs(v(2)) < n4::abs(v(at)) ) {
          at = 2;
        }
        return at;
      }

    } // namespace impl

    template<typename VecT>
    inline Matrix4f frameFromZ(const VecT& _z)
    {
      const VecT   z = n4::normalize(_z);
      const size_t r = impl::absMinAt(z); // axis of rotation
      const VecT   x = r == 0
          ? n4::normalize(VecT{0, -z(2), z(1)})
          : r == 1
            ? n4::normalize(VecT{z(2), 0, -z(0)})
            : n4::normalize(VecT{-z(1), z(0), 0});
      const VecT y = n4::normalize(n4::cross(z, x));
      return Matrix4f{
        x(0), y(0), z(0), 0,
        x(1), y(1), z(1), 0,
        x(2), y(2), z(2), 0,
        0   , 0   , 0   , 1
      };
    }

    /*
      * NOTE:
      * lookAt() uses OpenGL's camera conventions (AKA is right-handed):
      * - the viewer looks along the negative z axis
      * - x points right
      * - y points up
      */
    template<typename VerT, typename DirT>
    inline Matrix4f lookAt(const VerT& from, const VerT& to, const DirT& up)
    {
      using dir_traits = typename DirT::traits_type;
      const DirT z = expr_cast<dir_traits>(n4::direction(to, from)); // Looking along the NEGATIVE z axis!
      const DirT x = n4::normalize(n4::cross(up, z));
      const DirT y = n4::normalize(n4::cross(z, x));
      return Matrix4f{
        x(0), y(0), z(0), from(0),
        x(1), y(1), z(1), from(1),
        x(2), y(2), z(2), from(2),
        0   , 0   , 0   , 1
      };
    }

  } // namespace util

} // namespace n4

#endif // N4_UTIL_H
