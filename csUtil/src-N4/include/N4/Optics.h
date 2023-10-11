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

#ifndef N4_OPTICS_H
#define N4_OPTICS_H

#include <algorithm>

#include <N4/ExprCast.h>
#include <N4/Functions.h>
#include <N4/Math.h>

namespace n4 {

  namespace optics {

    /*
     * Optics API:
     *
     * The API models the transmission of light from an incident media 'i'
     * into a media 't'. Throughout this API the following conventions hold:
     *
     * I   := The incident vector following the direction of light.
     * N   := The normal vector of the bounding surface pointing
     *        from the media 't' to the media 'i'.
     * eta := The ratio of the indices of refraction; eta = etai/etat.
     *
     * NOTE 1: All input vectors are considered normalized AKA having a length of 1!
     *
     * NOTE 2: Properly setting up the geometry and refractive index is beyond
     *         the scope of this API and needs to be done by the caller!
     */

    constexpr real_t ZERO = 0;
    constexpr real_t  ONE = 1;
    constexpr real_t  TWO = 2;

    // cf. GLSL v4.60, 8.5. Geometric Functions
    template<typename Direction, typename Normal>
    inline Direction reflect(const Direction& I, const Normal& N)
    {
      using dir_traits = typename Direction::traits_type;
      const real_t cosTi = n4::dot(I, expr_cast<dir_traits>(N));
      return I - TWO*cosTi*expr_cast<dir_traits>(N);
    }

    // cf. GLSL v4.60, 8.5. Geometric Functions
    template<typename Direction, typename Normal>
    inline Direction refract(const Direction& I, const Normal& N, const real_t eta)
    {
      using dir_traits = typename Direction::traits_type;
      const real_t  cosTi = n4::dot(I, expr_cast<dir_traits>(N));
      const real_t sin2Ti = std::max<real_t>(0, ONE - cosTi*cosTi);
      const real_t sin2Tt = eta*eta*sin2Ti; // Snell's Law
      const real_t cos2Tt = ONE - sin2Tt;
      return cos2Tt >= ZERO // Handle Total Internal Reflection
          ? eta*I - (eta*cosTi + n4::sqrt(cos2Tt))*expr_cast<dir_traits>(N)
          : Direction();
    }

  } // namespace optics

} // namespace n4

#endif // N4_OPTICS_H
