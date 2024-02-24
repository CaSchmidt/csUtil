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

#include <cs/Concurrent/Map.h>
#include <cs/Concurrent/Reduce.h>

namespace cs {

  ////// Implementation //////////////////////////////////////////////////////

  namespace impl_mapreduce {

    // Constants /////////////////////////////////////////////////////////////

    constexpr auto ASYNC = std::launch::async;

    constexpr std::size_t ONE = 1;

  } // namespace impl_mapreduce

  ////// Public //////////////////////////////////////////////////////////////

  // (Sorted) Map-Reduce /////////////////////////////////////////////////////

  template<typename T, typename ForwardIt, typename MapToFunc, typename ReduceFunc, typename FoldFunc>
  T blockingMapReduce(const std::size_t numThreads,
                      ForwardIt first, ForwardIt last,
                      MapToFunc&& mapTo,
                      ReduceFunc&& reduce, FoldFunc&& fold)
  {
    using namespace impl_mapreduce;

    using mapped_t = std::invoke_result_t<MapToFunc,impl_iter::iter_const_reference<ForwardIt>>;
    using Mapped = std::vector<mapped_t>;

    Mapped mapped;
    if( numThreads < ONE  ||  !resize(&mapped, distance0(first, last)) ) {
      return T{};
    }

    // (1) Map Items /////////////////////////////////////////////////////////

    blockingMapSorted(numThreads, mapped.begin(), mapped.end(),
                      first, last, std::forward<MapToFunc>(mapTo));

    // (2) Reduce Items //////////////////////////////////////////////////////

    const T result = blockingReduce<T>(numThreads, mapped.begin(), mapped.end(),
                                       std::forward<ReduceFunc>(reduce),
                                       std::forward<FoldFunc>(fold));

    return result;
  }

  template<typename T, typename ForwardIt, typename MapToFunc, typename ReduceFunc>
  T blockingMapReduce(const std::size_t numThreads,
                      ForwardIt first, ForwardIt last,
                      MapToFunc&& mapTo, ReduceFunc&& reduce)
  {
    return blockingMapReduce<T>(numThreads, first, last,
                                std::forward<MapToFunc>(mapTo),
                                std::forward<ReduceFunc>(reduce), std::plus<void>{});
  }

  template<typename T, typename ForwardIt, typename MapToFunc, typename ReduceFunc, typename FoldFunc>
  [[nodiscard]] std::future<T> mapReduce(const std::size_t numThreads,
                                         ForwardIt first, ForwardIt last,
                                         MapToFunc&& mapTo,
                                         ReduceFunc&& reduce, FoldFunc&& fold)
  {
    using namespace impl_mapreduce;

    return std::async(ASYNC, blockingMapReduce<T,ForwardIt,MapToFunc,ReduceFunc,FoldFunc>,
                      numThreads, first, last,
                      std::forward<MapToFunc>(mapTo),
                      std::forward<ReduceFunc>(reduce), std::forward<FoldFunc>(fold));
  }

  template<typename T, typename ForwardIt, typename MapToFunc, typename ReduceFunc>
  [[nodiscard]] std::future<T> mapReduce(const std::size_t numThreads,
                                         ForwardIt first, ForwardIt last, MapToFunc&& mapTo,
                                         ReduceFunc&& reduce)
  {
    using namespace impl_mapreduce;

    return std::async(ASYNC, blockingMapReduce<T,ForwardIt,MapToFunc,ReduceFunc>,
                      numThreads, first, last,
                      std::forward<MapToFunc>(mapTo), std::forward<ReduceFunc>(reduce));
  }

} // namespace cs
