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

#include <future>

#include <cs/Concurrent/ThreadPool.h>
#include <cs/Core/Container.h>
#include <cs/Core/Iterator.h>

namespace cs {

  ////// Implementation //////////////////////////////////////////////////////

  namespace impl_map {

    using namespace impl_iter;

    // Type Traits ///////////////////////////////////////////////////////////

    /*
     * Syntax of map() function:
     *
     * U map(T& item)
     *
     * NOTE: Return value is never used!
     */

    template<typename MapFunc, typename IterT>
    using is_map = std::bool_constant<
    std::is_invocable_v<MapFunc,iter_reference<IterT>>
    >;

    template<typename MapFunc, typename IterT>
    inline constexpr bool is_map_v = is_map<MapFunc,IterT>::value;

    /*
     * Syntax of mapTo() function:
     *
     * U mapTo(const T& item)
     */

    template<typename OutputIt, typename MapToFunc, typename InputIt>
    using is_mapTo = std::bool_constant<
    iter_is_assignable_v<OutputIt,std::invoke_result_t<MapToFunc,iter_const_reference<InputIt>>>
    >;

    template<typename OutputIt, typename MapToFunc, typename InputIt>
    inline constexpr bool is_mapTo_v = is_mapTo<OutputIt,MapToFunc,InputIt>::value;

    // Constants /////////////////////////////////////////////////////////////

    constexpr auto ASYNC = std::launch::async;

    // Implementation ////////////////////////////////////////////////////////

    template<typename IterT>
    inline bool isDone(IterT first, IterT last)
    {
      return first == last;
    }

    template<typename OutputIt, typename InputIt>
    inline bool isDone(OutputIt destFirst, OutputIt destLast,
                       InputIt srcFirst, InputIt srcLast)
    {
      return isDone(destFirst, destLast)  ||  isDone(srcFirst, srcLast);
    }

  } // namespace impl_map

  ////// Public //////////////////////////////////////////////////////////////

  // In-Place Map ////////////////////////////////////////////////////////////

  template<typename MapFunc, typename IterT>
  concept IsMapFunction = impl_map::is_map_v<MapFunc,IterT>;

  template<typename ForwardIt, typename MapFunc>
  requires IsMapFunction<MapFunc,ForwardIt>
  void blockingMap(const std::size_t numThreads,
                   ForwardIt first, ForwardIt last,
                   MapFunc&& map)
  {
    using namespace impl_map;

    // (1) Create ThreadPool /////////////////////////////////////////////////

    ThreadPool pool(0);
    if( !pool.start(numThreads) ) {
      return;
    }

    // (2) Dispatch Work /////////////////////////////////////////////////////

    for(; first != last; ++first) {
      pool.dispatch(std::bind(std::forward<MapFunc>(map), std::ref(*first)));
    }

    // (3) Wait //////////////////////////////////////////////////////////////

    pool.finish();
  }

  template<typename ForwardIt, typename MapFunc>
  requires IsMapFunction<MapFunc,ForwardIt>
  [[nodiscard]] std::future<void> map(const std::size_t numThreads,
                                      ForwardIt first, ForwardIt last,
                                      MapFunc&& map)
  {
    using namespace impl_map;

    return std::async(ASYNC, blockingMap<ForwardIt,MapFunc>,
                      numThreads, first, last, std::forward<MapFunc>(map));
  }

  ////// Unsorted Map ////////////////////////////////////////////////////////

  template<typename OutputIt, typename MapToFunc, typename InputIt>
  concept IsMapToFunction = impl_map::is_mapTo_v<OutputIt,MapToFunc,InputIt>;

  template<typename OutputIt, typename InputIt, typename MapToFunc>
  requires IsMapToFunction<OutputIt,MapToFunc,InputIt>
  void blockingMapUnsorted(const std::size_t numThreads,
                           OutputIt dest,
                           InputIt first, InputIt last,
                           MapToFunc&& mapTo)
  {
    using namespace impl_map;

    using  input_t = iter_value_type<InputIt>;
    using output_t = iter_value_type<OutputIt>;

    // (1) Create ThreadPool /////////////////////////////////////////////////

    ThreadPool pool(0);
    if( !pool.start(numThreads) ) {
      return;
    }

    // (2) Dispatch Work /////////////////////////////////////////////////////

    auto lambda_worker =
        [&](output_t& out, const input_t& in) -> void {
      out = std::invoke(std::forward<MapToFunc>(mapTo), in);
    };

    for(; first != last; ++dest, ++first) {
      pool.dispatch(std::bind(lambda_worker, std::ref(*dest), std::cref(*first)));
    }

    // (3) Wait //////////////////////////////////////////////////////////////

    pool.finish();
  }

  template<typename OutputIt, typename InputIt, typename MapToFunc>
  requires IsMapToFunction<OutputIt,MapToFunc,InputIt>
  [[nodiscard]] std::future<void> mapUnsorted(const std::size_t numThreads,
                                              OutputIt dest,
                                              InputIt first, InputIt last,
                                              MapToFunc&& mapTo)
  {
    using namespace impl_map;

    return std::async(ASYNC, blockingMapUnsorted<OutputIt,InputIt,MapToFunc>,
                      numThreads, dest,
                      first, last, std::forward<MapToFunc>(mapTo));
  }

  ////// Sorted Map //////////////////////////////////////////////////////////

  template<typename OutputIt, typename InputIt, typename MapToFunc>
  requires IsMapToFunction<OutputIt,MapToFunc,InputIt>
  void blockingMapSorted(const std::size_t numThreads,
                         OutputIt destFirst, OutputIt destLast,
                         InputIt srcFirst, InputIt srcLast,
                         MapToFunc&& mapTo)
  {
    using namespace impl_map;

    using  input_t = iter_value_type<InputIt>;
    using output_t = iter_value_type<OutputIt>;

    // (1) Create ThreadPool /////////////////////////////////////////////////

    ThreadPool pool(0);
    if( !pool.start(numThreads) ) {
      return;
    }

    // (2) Dispatch Work /////////////////////////////////////////////////////

    auto lambda_worker =
        [&](output_t& out, const input_t& in) -> void {
      out = std::invoke(std::forward<MapToFunc>(mapTo), in);
    };

    for(; !isDone(destFirst, destLast, srcFirst, srcLast);
        ++destFirst, ++srcFirst) {
      pool.dispatch(std::bind(lambda_worker, std::ref(*destFirst), std::cref(*srcFirst)));
    }

    // (3) Wait //////////////////////////////////////////////////////////////

    pool.finish();
  }

  template<typename OutputIt, typename InputIt, typename MapToFunc>
  requires IsMapToFunction<OutputIt,MapToFunc,InputIt>
  [[nodiscard]] std::future<void> mapSorted(const std::size_t numThreads,
                                            OutputIt destFirst, OutputIt destLast,
                                            InputIt srcFirst, InputIt srcLast,
                                            MapToFunc&& mapTo)
  {
    using namespace impl_map;

    return std::async(ASYNC, blockingMapSorted<OutputIt,InputIt,MapToFunc>,
                      numThreads, destFirst, destLast,
                      srcFirst, srcLast, std::forward<MapToFunc>(mapTo));
  }

} // namespace cs
