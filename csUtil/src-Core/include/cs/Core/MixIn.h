/****************************************************************************
** Copyright (c) 2024, Carsten Schmidt. All rights reserved.
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

#include <cs/Core/CRTPbase.h>

namespace cs {

  ////// Read/Write Accessors ////////////////////////////////////////////////

  /*
   * NOTE: 'DerivedT' must implement the following functions
   *       to use the accessors:
   *
   * const T& DerivedT::impl_cref() const;
   * T& DerivedT::impl_ref();
   */

  /*
   * NOTE: A more conscious choice is to split member-access across the
   *       read/write MixIns. However, overload resolution prevents this
   *       when inheriting both read & write access.
   */
  template<typename DerivedT, typename ValueT>
  struct MemberAccessMixIn : CRTPbase<MemberAccessMixIn<DerivedT,ValueT>> {
    using value_type = ValueT;

    inline const value_type *operator->() const
    {
      return &this->as_derived().impl_cref();
    }

    inline value_type *operator->()
    {
      return &this->as_derived().impl_ref();
    }
  };

  template<typename DerivedT, typename ValueT>
  struct ReadAccessMixIn : CRTPbase<ReadAccessMixIn<DerivedT,ValueT>> {
    using value_type = ValueT;

    inline operator value_type() const
    {
      return this->as_derived().impl_cref();
    }
  };

  /*
   * NOTE: 'DerivedT' must explicitly reference this interface:
   *
   * using WriteAccessMixIn<...>::operator=;
   */
  template<typename DerivedT, typename ValueT>
  struct WriteAccessMixIn : CRTPbase<WriteAccessMixIn<DerivedT,ValueT>> {
    using value_type = ValueT;

    inline DerivedT& operator=(const value_type& value)
    {
      this->as_derived().impl_ref() = value;
      return this->as_derived();
    }
  };

} // namespace cs
