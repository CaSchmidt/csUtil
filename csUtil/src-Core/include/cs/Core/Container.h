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

#include <utility>

namespace cs {

  template<typename C>
  inline bool resize(C& container,
                     const typename C::size_type count)
  {
    constexpr typename C::size_type ONE = 1;

    container.clear();

    if( count < ONE ) {
      return false;
    }

    try {
      container.resize(count);
    } catch(...) {
      container.clear();
      return false;
    }

    return container.size() == count;
  }

  template<typename C>
  inline bool resize(C& container,
                     const typename C::size_type count,
                     const typename C::value_type& value)
  {
    constexpr typename C::size_type ONE = 1;

    container.clear();

    if( count < ONE ) {
      return false;
    }

    try {
      container.resize(count, value);
    } catch(...) {
      container.clear();
      return false;
    }

    return container.size() == count;
  }

  template<typename C>
  inline typename C::value_type takeFirst(C& container,
                                          const typename C::value_type& defValue = typename C::value_type{})
  {
    if( container.empty() ) {
      return defValue;
    }

    typename C::value_type result{};
    try {
      result = std::move(container.front());
      container.pop_front();
    } catch(...) {
      return defValue;
    }

    return result;
  }

} // namespace cs
