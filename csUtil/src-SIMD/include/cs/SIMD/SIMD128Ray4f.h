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

#include <cs/SIMD/SIMD128Vec4f.h>

namespace cs {

  namespace simd {

    /*
     * NOTE: Intersection of a ray with an axis-aligned bounding box.
     *
     * Definition: ray(t) = org + t*dir, t := [0, tRayMax)
     *
     * Computing the parameter value 't' of a ray intersecting a plane
     * perpendicular to the elementary axes; e.g. the plane located at
     * x := minX with the normal (1,0,0):
     *
     * minX = orgX + tMinX*dirX
     *
     * Solving for tMinX:
     *
     * tMinX = (minX - orgX)/dirX
     *
     * The ray intersects the plane if:
     *
     * 0 <= tMinX  AND  tMinX < tRayMax
     *
     * Similar equations can be derived for the remaining five planes;
     * e.g. maxX, minY, maxY, ...
     *
     * Using SIMD instructions, tMin and tMax can be computed in parallel for
     * all axes. The ray intersects an axis-aligned bounding box if ALL
     * dimensions of tMin OR tMax satisfy the condition above.
     */

    ////// Ray Axis Aligned Bounding Box Intersection ////////////////////////

    template<typename S> requires is_simd128x4f_v<S>
    inline bool intersectRayAABBox(const typename S::block_type& bbMin,
                                   const typename S::block_type& bbMax,
                                   const typename S::block_type& rayOrg,
                                   const typename S::block_type& rayDir,
                                   const typename S::value_type rayMax)
    {
      using block_t = typename S::block_type;

      constexpr int XYZ_MASK = impl_vec4f::CMP_MASK<false>();

      // (0) Sanity Check: Direction is not Zero /////////////////////////////

      const int result_mask = S::cmp_mask(S::cmp_neq(rayDir, S::zero())) & XYZ_MASK;
      if( result_mask == 0 ) {
        return false;
      }

      // (1) Compute tMin/tMax ///////////////////////////////////////////////

      const block_t tMin = S::div(S::sub(bbMin, rayOrg), rayDir);
      const block_t tMax = S::div(S::sub(bbMax, rayOrg), rayDir);

      const block_t tRayMax = S::set(rayMax);

      // (2) Intersection Condition; e.g. 0 <= tMin AND tMin < tRayMax ///////

      const block_t condMin = S::bit_and(S::cmp_leq(S::zero(), tMin),
                                         S::cmp_lt(tMin, tRayMax));
      const block_t condMax = S::bit_and(S::cmp_leq(S::zero(), tMax),
                                         S::cmp_lt(tMax, tRayMax));

      // (3) Min OR Max Condition Intersects AABB ////////////////////////////

      const int result = S::cmp_mask(S::bit_or(condMin, condMax));

      // (4) Align Conditions with Ray Direction /////////////////////////////

      return (result & result_mask) == result_mask;
    }

    template<typename S> requires is_simd128x4f_v<S>
    inline bool intersectRayAABBox(const typename S::block_type& bbMin,
                                   const typename S::block_type& bbMax,
                                   const typename S::block_type& rayOrg,
                                   const typename S::block_type& rayDir)
    {
      using block_t = typename S::block_type;

      constexpr int XYZ_MASK = impl_vec4f::CMP_MASK<false>();

      const int result_mask = S::cmp_mask(S::cmp_neq(rayDir, S::zero())) & XYZ_MASK;
      if( result_mask == 0 ) {
        return false;
      }

      const block_t tMin = S::div(S::sub(bbMin, rayOrg), rayDir);
      const block_t tMax = S::div(S::sub(bbMax, rayOrg), rayDir);

      const block_t condMin = S::cmp_leq(S::zero(), tMin);
      const block_t condMax = S::cmp_leq(S::zero(), tMax);

      const int result = S::cmp_mask(S::bit_or(condMin, condMax));

      return (result & result_mask) == result_mask;
    }

  } // namespace simd

} // namespace cs
