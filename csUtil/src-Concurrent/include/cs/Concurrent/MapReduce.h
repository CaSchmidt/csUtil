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
    impl_iter::iter_is_dereferenceable_v<OutputIt,std::invoke_result_t<MapToFunc,impl_iter::iter_const_reference<InputIt>>>
    >;

    template<typename OutputIt, typename MapToFunc, typename InputIt>
    inline constexpr bool is_mapTo_v = is_mapTo<OutputIt,MapToFunc,InputIt>::value;

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

    template<typename T>
    inline bool isValidReady(const std::future<T>& f)
    {
      using Clock = std::chrono::high_resolution_clock;

      constexpr auto READY = std::future_status::ready;

      return f.valid()  &&  f.wait_until(Clock::now()) == READY;
    }

  } // namespace impl_mapreduce

  ////// Constants ///////////////////////////////////////////////////////////

  constexpr std::chrono::milliseconds MAP_WAIT_ms{50};

  ////// Map /////////////////////////////////////////////////////////////////

  template<typename MapFunc, typename IterT>
  concept IsMapFunction = impl_mapreduce::is_map_v<MapFunc,IterT>;

  template<typename ForwardIt, typename MapFunc>
  requires IsMapFunction<MapFunc,ForwardIt>
  void blockingMap(const std::size_t numThreads,
                   ForwardIt first, ForwardIt last, MapFunc&& map,
                   const std::chrono::milliseconds& wait_ms = MAP_WAIT_ms)
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

    for(bool is_done = isDone(first, last); !is_done; ) {
      for(std::size_t i = 0; i < futures.size()  &&  !is_done; i++) {
        Future& future = futures[i];

        if( isValidReady(future) ) {
          future.get();
        }

        if( !future.valid() ) {
          future = std::async(ASYNC, std::forward<MapFunc>(map),
                              std::ref(*first));
          ++first;
        }

        is_done = isDone(first, last);
      } // For Each Future

      if( !is_done ) {
        std::this_thread::sleep_for(wait_ms);
      }
    } // While Items Available

    // (2) Wait for Threads to Finish ////////////////////////////////////////

    for(bool is_finished = false; !is_finished; ) {
      std::size_t cntFinished = 0;

      for(Future& future : futures) {
        if( isValidReady(future) ) {
          future.get();
        }

        if( !future.valid() ) {
          cntFinished++;
        }
      } // For Each Future

      is_finished = cntFinished == futures.size();

      if( !is_finished ) {
        std::this_thread::sleep_for(wait_ms);
      }
    } // While Not Finished
  }

  template<typename ForwardIt, typename MapFunc>
  requires IsMapFunction<MapFunc,ForwardIt>
  [[nodiscard]] std::future<void> map(const std::size_t numThreads,
                                      ForwardIt first, ForwardIt last, MapFunc&& map,
                                      const std::chrono::milliseconds& wait_ms = MAP_WAIT_ms)
  {
    using namespace impl_mapreduce;

    return std::async(ASYNC, blockingMap<ForwardIt,MapFunc>,
                      numThreads, first, last, std::forward<MapFunc>(map), wait_ms);
  }

  ////// (Unsorted) Map //////////////////////////////////////////////////////

  template<typename OutputIt, typename MapToFunc, typename InputIt>
  concept IsMapToFunction = impl_mapreduce::is_mapTo_v<OutputIt,MapToFunc,InputIt>;

  template<typename OutputIt, typename InputIt, typename MapToFunc>
  requires IsMapToFunction<OutputIt,MapToFunc,InputIt>
  void blockingMapUnsorted(const std::size_t numThreads,
                           OutputIt dest, InputIt first, InputIt last, MapToFunc&& mapTo,
                           const std::chrono::milliseconds& wait_ms = MAP_WAIT_ms)
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

    for(bool is_done = isDone(first, last); !is_done; ) {
      for(std::size_t i = 0; i < futures.size()  &&  !is_done; i++) {
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

        is_done = isDone(first, last);
      } // For Each Future

      if( !is_done ) {
        std::this_thread::sleep_for(wait_ms);
      }
    } // While Items Available

    // (2) Wait for Threads to Finish ////////////////////////////////////////

    for(bool is_finished = false; !is_finished; ) {
      std::size_t cntFinished = 0;

      for(Future& future : futures) {
        if( isValidReady(future) ) {
          *dest = future.get();
          ++dest;
        }

        if( !future.valid() ) {
          cntFinished++;
        }
      } // For Each Future

      is_finished = cntFinished == futures.size();

      if( !is_finished ) {
        std::this_thread::sleep_for(wait_ms);
      }
    } // While Not Finished
  }

  template<typename OutputIt, typename InputIt, typename MapToFunc>
  requires IsMapToFunction<OutputIt,MapToFunc,InputIt>
  [[nodiscard]] std::future<void> mapUnsorted(const std::size_t numThreads,
                                              OutputIt dest, InputIt first, InputIt last, MapToFunc&& mapTo,
                                              const std::chrono::milliseconds& wait_ms = MAP_WAIT_ms)
  {
    using namespace impl_mapreduce;

    return std::async(ASYNC, blockingMapUnsorted<OutputIt,InputIt,MapToFunc>,
                      numThreads, dest, first, last, std::forward<MapToFunc>(mapTo), wait_ms);
  }

  ////// (Sorted) Map ////////////////////////////////////////////////////////

  template<typename OutputIt, typename InputIt, typename MapToFunc>
  requires IsMapToFunction<OutputIt,MapToFunc,InputIt>
  void blockingMapSorted(const std::size_t numThreads,
                         OutputIt destFirst, OutputIt destLast,
                         InputIt srcFirst, InputIt srcLast, MapToFunc&& mapTo,
                         const std::chrono::milliseconds& wait_ms = MAP_WAIT_ms)
  {
    using namespace impl_mapreduce;

    using map_result_type = std::invoke_result_t<MapToFunc,impl_iter::iter_const_reference<InputIt>>;
    using Pair = std::pair<std::future<map_result_type>,OutputIt>;
    using Pairs = std::vector<Pair>;

    Pairs pairs;
    if( numThreads < ONE  ||  !resize(&pairs, numThreads) ) {
      return;
    }

    // (1) Run Threads ///////////////////////////////////////////////////////

    for(bool is_done = isDone(destFirst, destLast, srcFirst, srcLast); !is_done; ) {
      for(std::size_t i = 0; i < pairs.size()  &&  !is_done; i++) {
        auto& [future, dest] = pairs[i];
        {}

        if( isValidReady(future) ) {
          *dest = future.get();
        }

        if( !future.valid() ) {
          dest = destFirst;
          future = std::async(ASYNC, std::forward<MapToFunc>(mapTo),
                              std::cref(*srcFirst));

          ++destFirst;
          ++srcFirst;
        }

        is_done = isDone(destFirst, destLast, srcFirst, srcLast);
      } // For Each Future

      if( !is_done ) {
        std::this_thread::sleep_for(wait_ms);
      }
    } // While Items Available

    // (2) Wait for Threads to Finish ////////////////////////////////////////

    for(bool is_finished = false; !is_finished; ) {
      std::size_t cntFinished = 0;

      for(Pair& pair : pairs) {
        auto& [future, dest] = pair;
        {}

        if( isValidReady(future) ) {
          *dest = future.get();
        }

        if( !future.valid() ) {
          cntFinished++;
        }
      } // For Each Future

      is_finished = cntFinished == pairs.size();

      if( !is_finished ) {
        std::this_thread::sleep_for(wait_ms);
      }
    } // While Not Finished
  }

  template<typename OutputIt, typename InputIt, typename MapToFunc>
  requires IsMapToFunction<OutputIt,MapToFunc,InputIt>
  [[nodiscard]] std::future<void> mapSorted(const std::size_t numThreads,
                                            OutputIt destFirst, OutputIt destLast,
                                            InputIt srcFirst, InputIt srcLast, MapToFunc&& mapTo,
                                            const std::chrono::milliseconds& wait_ms = MAP_WAIT_ms)
  {
    using namespace impl_mapreduce;

    return std::async(ASYNC, blockingMapSorted<OutputIt,InputIt,MapToFunc>,
                      numThreads, destFirst, destLast, srcFirst, srcLast, std::forward<MapToFunc>(mapTo), wait_ms);
  }

} // namespace cs
