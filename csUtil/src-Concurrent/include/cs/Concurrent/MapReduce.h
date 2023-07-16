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

#include <cstddef>

#include <future>
#include <utility>
#include <vector>

#include <cs/Core/Container.h>
#include <cs/Core/Iterator.h>

namespace cs {

  namespace impl_mapreduce {

    // Constants /////////////////////////////////////////////////////////////

    constexpr auto ASYNC = std::launch::async;

    constexpr std::size_t ONE = 1;

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
    std::is_invocable_v<MapFunc,impl_iter::iter_reference<IterT>>
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
    std::is_invocable_r_v<impl_iter::iter_value_type<OutputIt>,MapToFunc,impl_iter::iter_const_reference<InputIt>>
    >;

    template<typename OutputIt, typename MapToFunc, typename InputIt>
    inline constexpr bool is_mapTo_v = is_mapTo<OutputIt,MapToFunc,InputIt>::value;

    // Implementation ////////////////////////////////////////////////////////

    template<typename T>
    inline bool isValidReady(const std::future<T>& f)
    {
      constexpr auto READY = std::future_status::ready;
      constexpr auto TIMEOUT = std::chrono::milliseconds{50};

      return f.valid()  &&  f.wait_for(TIMEOUT) == READY;
    }

  } // namespace impl_mapreduce

  ////// Map /////////////////////////////////////////////////////////////////

  template<typename MapFunc, typename IterT>
  concept IsMapFunction = impl_mapreduce::is_map_v<MapFunc,IterT>;

  template<typename ForwardIt, typename MapFunc>
  requires IsMapFunction<MapFunc,ForwardIt>
  void blockingMap(const std::size_t numThreads,
                   ForwardIt first, ForwardIt last, MapFunc&& map)
  {
    using namespace impl_mapreduce;

    using map_result_type = std::invoke_result_t<MapFunc,impl_iter::iter_reference<ForwardIt>>;
    using Futures = std::vector<std::future<map_result_type>>;
    using Future = typename Futures::value_type;

    Futures futures;
    if( numThreads < ONE  ||  !cs::resize(&futures, numThreads) ) {
      return;
    }

    // (1) Run Threads ///////////////////////////////////////////////////////

    while( first != last ) {
      for(std::size_t i = 0; i < futures.size()  &&  first != last; i++) {
        Future& future = futures[i];

        if( isValidReady(future) ) {
          future.get();
        }

        if( !future.valid() ) {
          future = std::async(ASYNC, std::forward<MapFunc>(map),
                              std::ref(*first));
          ++first;
        }
      } // For Each Future
    }

    // (2) Wait for Threads to Finish ////////////////////////////////////////

    std::size_t cntFinished = 0;
    while( cntFinished != futures.size() ) {
      cntFinished = 0;
      for(Future& future : futures) {
        if( isValidReady(future) ) {
          future.get();
        }

        if( !future.valid() ) {
          cntFinished++;
        }
      } // For Each Future
    }
  }

  template<typename ForwardIt, typename MapFunc>
  requires IsMapFunction<MapFunc,ForwardIt>
  [[nodiscard]] std::future<void> map(const std::size_t numThreads,
                                      ForwardIt first, ForwardIt last, MapFunc&& map)
  {
    using namespace impl_mapreduce;

    return std::async(ASYNC, blockingMap<ForwardIt,MapFunc>,
                      numThreads, first, last, std::forward<MapFunc>(map));
  }

  ////// (Unsorted) Map //////////////////////////////////////////////////////

  template<typename OutputIt, typename MapToFunc, typename InputIt>
  concept IsMapToFunction = impl_mapreduce::is_mapTo_v<OutputIt,MapToFunc,InputIt>;

  template<typename OutputIt, typename InputIt, typename MapToFunc>
  // requires IsMapToFunction<OutputIt,MapToFunc,InputIt>
  void blockingMapUnsorted(const std::size_t numThreads,
                           OutputIt dest, InputIt first, InputIt last, MapToFunc&& mapTo)
  {
    using namespace impl_mapreduce;

    using map_result_type = std::invoke_result_t<MapToFunc,impl_iter::iter_const_reference<InputIt>>;
    using Futures = std::vector<std::future<map_result_type>>;
    using Future = typename Futures::value_type;

    Futures futures;
    if( numThreads < ONE  ||  !resize(&futures, numThreads) ) {
      return;
    }

    // (1) Run Threads ///////////////////////////////////////////////////////

    while( first != last ) {
      for(std::size_t i = 0; i < futures.size()  &&  first != last; i++) {
        Future& future = futures[i];

        if( isValidReady(future) ) {
          *dest = future.get();
          ++dest;
        }

        if( !future.valid() ) {
          future = std::async(ASYNC, std::forward<MapToFunc>(mapTo),
                              std::cref(*first));
          ++first;
        }
      } // For Each Future
    }

    // (2) Wait for Threads to Finish ////////////////////////////////////////

    std::size_t cntFinished = 0;
    while( cntFinished != futures.size() ) {
      cntFinished = 0;
      for(Future& future : futures) {
        if( isValidReady(future) ) {
          *dest = future.get();
          ++dest;
        }

        if( !future.valid() ) {
          cntFinished++;
        }
      } // For Each Future
    }
  }

  template<typename OutputIt, typename InputIt, typename MapToFunc>
  // requires IsMapToFunction<OutputIt,MapToFunc,InputIt>
  [[nodiscard]] std::future<void> mapUnsorted(const std::size_t numThreads,
                                              OutputIt dest, InputIt first, InputIt last, MapToFunc&& mapTo)
  {
    using namespace impl_mapreduce;

    return std::async(ASYNC, blockingMapUnsorted<OutputIt,InputIt,MapToFunc>,
                      numThreads, dest, first, last, std::forward<MapToFunc>(mapTo));
  }

} // namespace cs
