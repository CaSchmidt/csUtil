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

#include <cs/Concurrent/MapReduceImpl.h>
#include <cs/Concurrent/ThreadPool.h>

namespace cs::concurrent {

  ////// Public //////////////////////////////////////////////////////////////

  // In-Place Map ////////////////////////////////////////////////////////////

  template<typename MapFunc, typename IterT>
  concept IsMapFunction = impl_mapreduce::is_map_v<MapFunc,IterT>;

  template<typename InputIt, typename MapFunc>
  requires IsMapFunction<MapFunc,InputIt>
  void map(const std::size_t numThreads,
           InputIt first, InputIt last,
           MapFunc&& func)
  {
    using namespace impl_mapreduce;

    // (1) Create ThreadPool /////////////////////////////////////////////////

    ThreadPool pool(0);
    if( !pool.start(numThreads) ) {
      return;
    }

    // (2) Dispatch Work /////////////////////////////////////////////////////

    for(; first != last; ++first) {
      pool.dispatch(std::bind(std::forward<MapFunc>(func), std::ref(*first)));
    }

    // (3) Wait //////////////////////////////////////////////////////////////

    pool.finish();
  }

  template<typename InputIt, typename MapFunc>
  requires IsMapFunction<MapFunc,InputIt>
  [[nodiscard]] std::future<void> mapAsync(const std::size_t numThreads,
                                           InputIt first, InputIt last,
                                           MapFunc&& func)
  {
    using namespace impl_mapreduce;

    return std::async(ASYNC, map<InputIt,MapFunc>,
                      numThreads, first, last, std::forward<MapFunc>(func));
  }

  // Sorted Map //////////////////////////////////////////////////////////////

  template<typename OutputIt, typename MapToFunc, typename InputIt>
  concept IsMapToFunction = impl_mapreduce::is_mapTo_v<OutputIt,MapToFunc,InputIt>;

  template<typename OutputIt, typename InputIt, typename MapToFunc>
  requires IsMapToFunction<OutputIt,MapToFunc,InputIt>
  void mapSorted(const std::size_t numThreads,
                 OutputIt destFirst, OutputIt destLast,
                 InputIt srcFirst, InputIt srcLast,
                 MapToFunc&& mapTo)
  {
    using namespace impl_mapreduce;

    using Context = MapTo<OutputIt,MapToFunc,InputIt>;

    // (1) Create ThreadPool /////////////////////////////////////////////////

    ThreadPool pool(0);
    if( !pool.start(numThreads) ) {
      return;
    }

    // (2) Dispatch Work /////////////////////////////////////////////////////

    Context ctx(std::forward<MapToFunc>(mapTo));

    for(; !isDone(destFirst, destLast, srcFirst, srcLast);
        ++destFirst, ++srcFirst) {
      pool.dispatch(std::bind(ctx, std::ref(*destFirst), std::cref(*srcFirst)));
    }

    // (3) Wait //////////////////////////////////////////////////////////////

    pool.finish();
  }

  template<typename OutputIt, typename InputIt, typename MapToFunc>
  requires IsMapToFunction<OutputIt,MapToFunc,InputIt>
  [[nodiscard]] std::future<void> mapSortedAsync(const std::size_t numThreads,
                                                 OutputIt destFirst, OutputIt destLast,
                                                 InputIt srcFirst, InputIt srcLast,
                                                 MapToFunc&& mapTo)
  {
    using namespace impl_mapreduce;

    return std::async(ASYNC, mapSorted<OutputIt,InputIt,MapToFunc>,
                      numThreads, destFirst, destLast,
                      srcFirst, srcLast, std::forward<MapToFunc>(mapTo));
  }

  // Unsorted Map ////////////////////////////////////////////////////////////

  template<typename OutputIt, typename InputIt, typename MapToFunc>
  requires IsMapToFunction<OutputIt,MapToFunc,InputIt>
  void mapUnsorted(const std::size_t numThreads,
                   OutputIt d_first,
                   InputIt first, InputIt last,
                   MapToFunc&& mapTo)
  {
    using namespace impl_mapreduce;

    using Context = MapToLockedIter<OutputIt,MapToFunc,InputIt>;

    // (1) Create ThreadPool /////////////////////////////////////////////////

    ThreadPool pool(0);
    if( !pool.start(numThreads) ) {
      return;
    }

    // (2) Dispatch Work /////////////////////////////////////////////////////

    std::mutex mutex;
    Context ctx(std::forward<MapToFunc>(mapTo), mutex);

    for(; first != last; ++d_first, ++first) {
      pool.dispatch(std::bind(ctx, d_first, std::cref(*first)));
    }

    // (3) Wait //////////////////////////////////////////////////////////////

    pool.finish();
  }

  template<typename OutputIt, typename InputIt, typename MapToFunc>
  requires IsMapToFunction<OutputIt,MapToFunc,InputIt>
  [[nodiscard]] std::future<void> mapUnsortedAsync(const std::size_t numThreads,
                                                   OutputIt d_first,
                                                   InputIt first, InputIt last,
                                                   MapToFunc&& mapTo)
  {
    using namespace impl_mapreduce;

    return std::async(ASYNC, mapUnsorted<OutputIt,InputIt,MapToFunc>,
                      numThreads, d_first,
                      first, last, std::forward<MapToFunc>(mapTo));
  }

} // namespace cs::concurrent
