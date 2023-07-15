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

#include <iterator>
#include <type_traits>

namespace cs {

  ////// Implementation //////////////////////////////////////////////////////

  namespace impl_iter { // TODO: Promote to public API?

    template<typename IterT>
    struct iterator_traits {
      using value_type = std::remove_cvref_t<decltype(*IterT())>;

      using pointer = std::add_pointer_t<value_type>;

      using const_pointer = std::add_const_t<pointer>;

      using reference = std::add_lvalue_reference_t<value_type>;

      using const_reference = std::add_const_t<reference>;
    };

    template<typename IterT>
    using iter_reference = typename iterator_traits<IterT>::reference;

    template<typename IterT>
    using iter_value_type = typename iterator_traits<IterT>::value_type;

  } // namespace impl_iter

  ////// Public //////////////////////////////////////////////////////////////

  template<typename IterT>
  inline IterT prev(IterT pos, IterT beg)
  {
    return pos != beg
        ? std::prev(pos, 1)
        : beg;
  }

  template<typename IterT>
  inline IterT next(IterT pos, IterT end)
  {
    return pos != end
        ? std::next(pos, 1)
        : end;
  }

} // namespace cs
