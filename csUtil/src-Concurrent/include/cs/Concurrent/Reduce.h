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
#include <vector>

#include <cs/Core/Container.h>
#include <cs/Core/Iterator.h>

namespace cs {

  ////// Implementation //////////////////////////////////////////////////////

  namespace impl_reduce {

    using namespace impl_iter;

    // Type Traits ///////////////////////////////////////////////////////////

    /*
     * Syntax of reduce() function:
     *
     * V reduce(T& reduced, const U& mapped)
     */

    template<typename ReduceFunc, typename T, typename IterT>
    using is_reduce = std::bool_constant<
    std::is_invocable_v<ReduceFunc,std::add_lvalue_reference_t<T>,iter_const_reference<IterT>>
    >;

    template<typename ReduceFunc, typename T, typename IterT>
    inline constexpr bool is_reduce_v = is_reduce<ReduceFunc,T,IterT>::value;

    /*
     * Syntax of fold() function:
     *
     * T fold(T, T)
     */

    template<typename FoldFunc, typename T>
    using is_fold = std::bool_constant<
    std::is_invocable_r_v<T,FoldFunc,T,T>
    >;

    template<typename FoldFunc, typename T>
    inline constexpr bool is_fold_v = is_fold<FoldFunc,T>::value;

    // Types /////////////////////////////////////////////////////////////////

    /*
     * Usage:
     * -> Number of Threads
     * -> Items per Thread AKA the "Stride"
     * -> Number of Items modulo Number of Threads AKA the "Remaining Items"
     */
    using ReduceInfo = std::tuple<std::size_t,std::size_t,std::size_t>;

    // Constants /////////////////////////////////////////////////////////////

    constexpr auto ASYNC = std::launch::async;
    constexpr std::size_t ONE = 1;

    // Implementation ////////////////////////////////////////////////////////

    inline ReduceInfo computeReduceInfo(const std::size_t numThreads,
                                        const std::size_t numItems)
    {
      if( numThreads < ONE  ||  numItems < ONE ) {
        return ReduceInfo{0, 0, 0};
      }

      if( numItems <= numThreads ) {
        return ReduceInfo{numItems, 1, 0};
      }

      const std::size_t stride = numItems/numThreads;
      const std::size_t remain = numItems%numThreads;

      return ReduceInfo{numThreads, stride, remain};
    }

    template<typename T, typename ForwardIt, typename ReduceFunc>
    T reduceRange(ForwardIt first, ForwardIt last, ReduceFunc&& reduce)
    {
      T result{};
      for(; first != last; ++first) {
        std::invoke(std::forward<ReduceFunc>(reduce),
                    std::ref(result), std::cref(*first));
      }
      return result;
    }

  } // namespace impl_reduce

  ////// Public //////////////////////////////////////////////////////////////

  // Sorted Recude ///////////////////////////////////////////////////////////

  template<typename ReduceFunc, typename T, typename IterT>
  concept IsReduceFunction = impl_reduce::is_reduce_v<ReduceFunc,T,IterT>;

  template<typename FoldFunc, typename T>
  concept IsFoldFunction = impl_reduce::is_fold_v<FoldFunc,T>;

  template<typename T, typename ForwardIt, typename ReduceFunc, typename FoldFunc>
  requires IsReduceFunction<ReduceFunc,T,ForwardIt>  &&  IsFoldFunction<FoldFunc,T>
  T blockingReduce(const std::size_t _numThreads,
                   ForwardIt first, ForwardIt last, ReduceFunc&& reduce,
                   FoldFunc&& fold)
  {
    using namespace impl_reduce;

    using Futures = std::vector<std::future<T>>;
    using  Future = typename Futures::value_type;

    const auto [numThreads, stride, remain] =
        computeReduceInfo(_numThreads, distance0(first, last));
    {}

    Futures futures;
    if( numThreads < ONE  ||  !resize(&futures, numThreads) ) {
      return T{};
    }

    // (1) Run Threads ///////////////////////////////////////////////////////

    for(std::size_t i = 0; i < numThreads; i++) {
      const ForwardIt last = i < remain
          ? std::next(first, stride + ONE)
          : std::next(first, stride);

      futures[i] = std::async(ASYNC, reduceRange<T,ForwardIt,ReduceFunc>,
                              first, last, std::forward<ReduceFunc>(reduce));

      first = last;
    }

    // (2) Fold Results //////////////////////////////////////////////////////

    T result{};
    for(Future& future : futures) {
      result = std::invoke(std::forward<FoldFunc>(fold),
                           std::move(result), future.get());
    }

    return result;
  }

  template<typename T, typename ForwardIt, typename ReduceFunc, typename FoldFunc>
  requires IsReduceFunction<ReduceFunc,T,ForwardIt>  &&  IsFoldFunction<FoldFunc,T>
  [[nodiscard]] std::future<T> reduce(const std::size_t numThreads,
                                      ForwardIt first, ForwardIt last, ReduceFunc&& reduce,
                                      FoldFunc&& fold)
  {
    using namespace impl_reduce;

    return std::async(ASYNC, blockingReduce<T,ForwardIt,ReduceFunc,FoldFunc>,
                      numThreads, first, last, std::forward<ReduceFunc>(reduce),
                      std::forward<FoldFunc>(fold));
  }

  template<typename T, typename ForwardIt, typename ReduceFunc>
  requires IsReduceFunction<ReduceFunc,T,ForwardIt>
  T blockingReduce(const std::size_t numThreads,
                   ForwardIt first, ForwardIt last, ReduceFunc&& reduce)
  {
    return blockingReduce<T>(numThreads, first, last, std::forward<ReduceFunc>(reduce),
                             std::plus<>{});
  }

  template<typename T, typename ForwardIt, typename ReduceFunc>
  requires IsReduceFunction<ReduceFunc,T,ForwardIt>
  [[nodiscard]] std::future<T> reduce(const std::size_t numThreads,
                                      ForwardIt first, ForwardIt last, ReduceFunc&& reduce)
  {
    using namespace impl_reduce;

    return std::async(ASYNC, blockingReduce<T,ForwardIt,ReduceFunc>,
                      numThreads, first, last, std::forward<ReduceFunc>(reduce));
  }

} // namespace cs
