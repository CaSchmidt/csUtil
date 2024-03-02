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

#include <cs/Concurrent/MapReduceUtil.h>
#include <cs/Concurrent/ThreadPool.h>

namespace cs::concurrent {

  ////// Public //////////////////////////////////////////////////////////////

  /*
   * Apply the Map-Reduce model to the result of dereferencing every iterator
   * in the range [first,last).
   *
   * The Map-Reduce operation is executed in two steps:
   *
   * Step 1: Apply the mapTo() function to the dereferenced iterator:
   *
   * M mapTo(const U&)
   *
   * Step 2: Reduce the mapped result of Step 1 using the reduce() function:
   *
   * V reduce(T&, const M&)
   *
   * The return type and value of the reduce() function is never used.
   *
   * Both steps are executed in the same thread.
   *
   * Access to the reduced result is protected using a mutex; no additional
   * synchronization needs to be implemented!
   */

  template<typename T, typename ReduceFunc, typename MapToFunc, typename InputIt>
  concept IsMapReduceFunction = impl_mapreduce::is_mapReduce_v<T,ReduceFunc,MapToFunc,InputIt>;

  // (Sorted) Map-Reduce /////////////////////////////////////////////////////

  template<typename T, typename InputIt, typename MapToFunc, typename ReduceFunc>
  requires IsMapReduceFunction<T,ReduceFunc,MapToFunc,InputIt>
  T blockingMapReduceSorted(const std::size_t numThreads,
                            InputIt first, InputIt last,
                            MapToFunc&& mapTo, ReduceFunc&& reduce)
  {
    using namespace impl_mapreduce;

    using Context = MapReduceSorted<T,ReduceFunc,MapToFunc,InputIt>;

    constexpr size_type INIT_ID = 0;

    // (1) Create ThreadPool /////////////////////////////////////////////////

    ThreadPool pool(0);
    if( !pool.start(numThreads) ) {
      return T();
    }

    // (2) Dispatch Work /////////////////////////////////////////////////////

    SyncCondition<size_type> sync(INIT_ID);
    Context ctx(std::forward<MapToFunc>(mapTo),
                std::forward<ReduceFunc>(reduce),
                sync);

    size_type id = INIT_ID;
    T result{};
    for(; first != last; ++first, id++) {
      pool.dispatch(std::bind(ctx, id, std::ref(result), std::cref(*first)));
    }

    // (3) Wait //////////////////////////////////////////////////////////////

    pool.finish();

    return result;
  }

  template<typename T, typename InputIt, typename MapToFunc, typename ReduceFunc>
  requires IsMapReduceFunction<T,ReduceFunc,MapToFunc,InputIt>
  [[nodiscard]] std::future<T> mapReduceSorted(const std::size_t numThreads,
                                               InputIt first, InputIt last,
                                               MapToFunc&& mapTo, ReduceFunc&& reduce)
  {
    using namespace impl_mapreduce;

    return std::async(ASYNC, blockingMapReduceSorted<T,InputIt,MapToFunc,ReduceFunc>,
                      numThreads, first, last,
                      std::forward<MapToFunc>(mapTo), std::forward<ReduceFunc>(reduce));
  }

  // (Unsorted) Map-Reduce ///////////////////////////////////////////////////

  template<typename T, typename InputIt, typename MapToFunc, typename ReduceFunc>
  requires IsMapReduceFunction<T,ReduceFunc,MapToFunc,InputIt>
  T blockingMapReduceUnsorted(const std::size_t numThreads,
                              InputIt first, InputIt last,
                              MapToFunc&& mapTo, ReduceFunc&& reduce)
  {
    using namespace impl_mapreduce;

    using Context = MapReduceUnsorted<T,ReduceFunc,MapToFunc,InputIt>;

    // (1) Create ThreadPool /////////////////////////////////////////////////

    ThreadPool pool(0);
    if( !pool.start(numThreads) ) {
      return T();
    }

    // (2) Dispatch Work /////////////////////////////////////////////////////

    std::mutex mutex;
    Context ctx(std::forward<MapToFunc>(mapTo),
                std::forward<ReduceFunc>(reduce),
                mutex);

    T result{};
    for(; first != last; ++first) {
      pool.dispatch(std::bind(ctx, std::ref(result), std::cref(*first)));
    }

    // (3) Wait //////////////////////////////////////////////////////////////

    pool.finish();

    return result;
  }

  template<typename T, typename InputIt, typename MapToFunc, typename ReduceFunc>
  requires IsMapReduceFunction<T,ReduceFunc,MapToFunc,InputIt>
  [[nodiscard]] std::future<T> mapReduceUnsorted(const std::size_t numThreads,
                                                 InputIt first, InputIt last,
                                                 MapToFunc&& mapTo, ReduceFunc&& reduce)
  {
    using namespace impl_mapreduce;

    return std::async(ASYNC, blockingMapReduceUnsorted<T,InputIt,MapToFunc,ReduceFunc>,
                      numThreads, first, last,
                      std::forward<MapToFunc>(mapTo), std::forward<ReduceFunc>(reduce));
  }

} // namespace cs::concurrent
