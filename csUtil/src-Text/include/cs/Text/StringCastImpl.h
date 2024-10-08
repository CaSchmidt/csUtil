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

#include <algorithm>
#include <string>

#include <cs/Core/CharUtil.h>
#include <cs/Core/Container.h>

namespace cs {

  namespace impl_string {

    template<typename NCharT, typename WCharT>
    inline std::basic_string<NCharT> narrow(const std::basic_string_view<WCharT>& text)
    {
      std::basic_string<NCharT> result;

      if( text.empty()  ||  !resize(result, text.size()) ) {
        return std::basic_string<NCharT>();
      }

      std::transform(text.begin(), text.end(), result.begin(), lambda_narrow<NCharT,WCharT>());

      return result;
    }

    template<typename WCharT, typename NCharT>
    inline std::basic_string<WCharT> widen(const std::basic_string_view<NCharT>& text)
    {
      std::basic_string<WCharT> result;

      if( text.empty()  ||  !resize(result, text.size()) ) {
        return std::basic_string<WCharT>();
      }

      std::transform(text.begin(), text.end(), result.begin(), lambda_widen<WCharT,NCharT>());

      return result;
    }

  } // namespace impl_string

} // namespace cs
