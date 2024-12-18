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

#include <algorithm>
#include <iterator>
#include <type_traits>

namespace cs {

  ////// Traits //////////////////////////////////////////////////////////////

  template<typename C, typename = void>
  struct is_reservable : std::false_type { };

  template<typename C>
  struct is_reservable<C,std::void_t<decltype((void)std::declval<C>().reserve(1),bool{})>> : std::true_type { };

  template<typename C>
  inline constexpr bool is_reservable_v = is_reservable<C>::value;

  template<typename C, typename = void>
  struct is_sortable : std::false_type { };

  template<typename C>
  struct is_sortable<C,std::void_t<decltype((void)std::declval<C>().sort(),bool{})>> : std::true_type { };

  template<typename C>
  inline constexpr bool is_sortable_v = is_sortable<C>::value;

  ////// Functions ///////////////////////////////////////////////////////////

  // reserve() Container of Capacity... //////////////////////////////////////

  template<typename C>
  inline std::enable_if_t<!is_reservable_v<C>,void> reserve(C& /*container*/,
                                                            const typename C::size_type /*cap*/)
  {
    // Container does not implement C::reserve()!
  }

  template<typename C>
  inline std::enable_if_t<is_reservable_v<C>,void> reserve(C& container,
                                                           const typename C::size_type cap)
  {
    try {
      container.reserve(cap);
    } catch( ... ) {
      container.clear();
    }
  }

  // resize() Container... ///////////////////////////////////////////////////

  template<typename C>
  inline bool resize(C& container,
                     const typename C::size_type count)
  {
    constexpr typename C::size_type ONE = 1;

    if( count < ONE ) {
      return false;
    }

    try {
      container.resize(count);
    } catch( ... ) {
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

    if( count < ONE ) {
      return false;
    }

    try {
      container.resize(count, value);
    } catch( ... ) {
      container.clear();
      return false;
    }

    return container.size() == count;
  }

  // sort() Container... /////////////////////////////////////////////////////

  template<typename C>
  inline std::enable_if_t<!is_sortable_v<C>,void> sort(C& container)
  {
    std::sort(container.begin(), container.end());
  }

  template<typename C, typename Compare>
  inline std::enable_if_t<!is_sortable_v<C>,void> sort(C& container,
                                                       Compare cmp)
  {
    std::sort(container.begin(), container.end(), cmp);
  }

  template<typename C>
  inline std::enable_if_t<is_sortable_v<C>,void> sort(C& container)
  {
    container.sort();
  }

  template<typename C, typename Compare>
  inline std::enable_if_t<is_sortable_v<C>,void> sort(C& container,
                                                      Compare cmp)
  {
    container.sort(cmp);
  }

  // takeFirst() Element of Container... /////////////////////////////////////

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
    } catch( ... ) {
      return defValue;
    }

    return result;
  }

  // takeIndex() Element of Container... /////////////////////////////////////

  template<typename C>
  inline typename C::value_type takeIndex(C& container,
                                          const typename C::iterator::difference_type index,
                                          const typename C::value_type defValue = typename C::value_type{})
  {
    using diff_type = typename C::iterator::difference_type;

    constexpr diff_type ZERO = 0;

    // TODO: Account for difference_type vs. size_type signedness?
    if( index < ZERO  ||  index >= static_cast<diff_type>(container.size()) ) {
      return defValue;
    }

    typename C::value_type result;
    try {
      auto iterIndex = std::next(container.begin(), index);
      result = std::move(*iterIndex);
      container.erase(iterIndex);
    } catch(...) {
      return defValue;
    }

    return result;
  }

  // List (extracted) Elements of Container... ///////////////////////////////

  template<template<typename ...> typename C, typename IterT, typename Extract>
  inline auto toSequence(IterT first, IterT last, Extract extract,
                         const bool do_sort = false, const bool do_unique = false)
  {
    using    diff_type = typename std::iterator_traits<IterT>::difference_type;
    using   value_type = typename std::iterator_traits<IterT>::value_type;
    using extract_type = std::invoke_result_t<Extract,value_type>;

    using Sequence = C<extract_type>;

    constexpr std::size_t ZERO = 0;

    const std::size_t count = std::max<diff_type>(0, std::distance(first, last));

    if( count == ZERO ) {
      return Sequence();
    }

    Sequence result;
    reserve(result, count);

    std::transform(first, last, std::back_inserter(result), extract);

    if( do_sort  ||  do_unique ) {
      sort(result);
    }

    if( do_unique ) {
      const auto junk = std::unique(result.begin(), result.end());
      result.erase(junk, result.end());
    }

    return result;
  }

} // namespace cs
