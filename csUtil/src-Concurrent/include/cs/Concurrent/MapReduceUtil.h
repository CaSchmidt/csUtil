/****************************************************************************
** Copyright (c) 2024, Carsten Schmidt. All rights reserved.
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

#include <cs/Concurrent/SyncCondition.h>
#include <cs/Core/Iterator.h>

namespace cs::concurrent {

  namespace impl_mapreduce {

    using namespace impl_iter;

    // Types /////////////////////////////////////////////////////////////////

    using size_type = std::size_t;

    // Type Traits ///////////////////////////////////////////////////////////

    /*
     * Syntax of map() function:
     *
     * U map(T& item)
     *
     * NOTE: Return value & type are never used!
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

    /*
     * Syntax of Map-Reduce functions:
     *
     * M mapTo(const T& item)
     * U reduce(R& reduced, const M& mapped)
     *
     * NOTE: reduce()'s return value & type are never used!
     */

    template<typename R, typename ReduceFunc, typename MapToFunc, typename InputIt>
    using is_mapReduce = std::bool_constant<
    std::is_invocable_v<ReduceFunc,
    std::add_lvalue_reference_t<R>,
    std::add_const_t<std::add_lvalue_reference_t<std::invoke_result_t<MapToFunc,iter_const_reference<InputIt>>>>>
    >;

    template<typename R, typename ReduceFunc, typename MapToFunc, typename InputIt>
    inline constexpr bool is_mapReduce_v = is_mapReduce<R,ReduceFunc,MapToFunc,InputIt>::value;

    // Constants /////////////////////////////////////////////////////////////

    inline constexpr auto ASYNC = std::launch::async;

    inline constexpr size_type ONE = 1;

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

    template<typename OutputIt, typename MapToFunc, typename InputIt>
    requires is_mapTo_v<OutputIt,MapToFunc,InputIt>
    class MapTo {
    public:
      using  input_t = iter_value_type<InputIt>;
      using output_t = iter_value_type<OutputIt>;

      MapTo(MapToFunc&& mapTo) noexcept
        : _mapTo{std::forward<MapToFunc>(mapTo)}
      {
      }

      MapTo(const MapTo& other) noexcept
        : _mapTo{std::forward<MapToFunc>(other._mapTo)}
      {
      }

      ~MapTo() noexcept
      {
      }

      void operator()(output_t& out, const input_t& in) const
      {
        out = std::invoke(std::forward<MapToFunc>(_mapTo), in);
      }

    private:
      MapToFunc&& _mapTo;
    };

    template<typename OutputIt, typename MapToFunc, typename InputIt>
    requires is_mapTo_v<OutputIt,MapToFunc,InputIt>
    class MapToLockedIter {
    public:
      using  input_t = iter_value_type<InputIt>;
      using mapped_t = std::invoke_result_t<MapToFunc,iter_const_reference<InputIt>>;

      MapToLockedIter(MapToFunc&& mapTo, std::mutex& mutex) noexcept
        : _mapTo{std::forward<MapToFunc>(mapTo)}
        , _mutex{mutex}
      {
      }

      MapToLockedIter(const MapToLockedIter& other) noexcept
        : _mapTo{std::forward<MapToFunc>(other._mapTo)}
        , _mutex{other._mutex}
      {
      }

      ~MapToLockedIter() noexcept
      {
      }

      void operator()(OutputIt dest, const input_t& in) const
      {
        const mapped_t mapped = std::invoke(std::forward<MapToFunc>(_mapTo), in);

        { // lock
          std::lock_guard<std::mutex> guard(_mutex);
          *dest = mapped;
        } // unlock
      }

    private:
      MapToFunc&& _mapTo;
      std::mutex& _mutex;
    };

    template<typename T, typename ReduceFunc, typename MapToFunc, typename InputIt>
    requires is_mapReduce_v<T,ReduceFunc,MapToFunc,InputIt>
    class MapReduceSorted {
    public:
      using  input_t = iter_value_type<InputIt>;
      using mapped_t = std::invoke_result_t<MapToFunc,iter_const_reference<InputIt>>;

      MapReduceSorted(MapToFunc&& mapTo, ReduceFunc&& reduce,
                      SyncCondition<size_type>& sync) noexcept
        : _mapTo{std::forward<MapToFunc>(mapTo)}
        , _reduce{std::forward<ReduceFunc>(reduce)}
        , _sync{sync}
      {
      }

      MapReduceSorted(const MapReduceSorted& other) noexcept
        : _mapTo{std::forward<MapToFunc>(other._mapTo)}
        , _reduce{std::forward<ReduceFunc>(other._reduce)}
        , _sync{other._sync}
      {
      }

      ~MapReduceSorted() noexcept
      {
      }

      void operator()(const size_type id, T& reduced, const input_t& in) const
      {
        const mapped_t mapped = std::invoke(std::forward<MapToFunc>(_mapTo), in);

        _sync.wait(id);

        std::invoke(std::forward<ReduceFunc>(_reduce), reduced, mapped);

        _sync.notify_all(id + ONE);
      }

    private:
      MapToFunc&&  _mapTo;
      ReduceFunc&& _reduce;
      SyncCondition<size_type>& _sync;
    };

    template<typename T, typename ReduceFunc, typename MapToFunc, typename InputIt>
    requires is_mapReduce_v<T,ReduceFunc,MapToFunc,InputIt>
    class MapReduceUnsorted {
    public:
      using  input_t = iter_value_type<InputIt>;
      using mapped_t = std::invoke_result_t<MapToFunc,iter_const_reference<InputIt>>;

      MapReduceUnsorted(MapToFunc&& mapTo, ReduceFunc&& reduce,
                        std::mutex& mutex) noexcept
        : _mapTo{std::forward<MapToFunc>(mapTo)}
        , _reduce{std::forward<ReduceFunc>(reduce)}
        , _mutex{mutex}
      {
      }

      MapReduceUnsorted(const MapReduceUnsorted& other) noexcept
        : _mapTo{std::forward<MapToFunc>(other._mapTo)}
        , _reduce{std::forward<ReduceFunc>(other._reduce)}
        , _mutex{other._mutex}
      {
      }

      ~MapReduceUnsorted() noexcept
      {
      }

      void operator()(T& reduced, const input_t& in) const
      {
        const mapped_t mapped = std::invoke(std::forward<MapToFunc>(_mapTo), in);

        { // lock
          std::lock_guard<std::mutex> guard(_mutex);
          std::invoke(std::forward<ReduceFunc>(_reduce), reduced, mapped);
        } // unlock
      }

    private:
      MapToFunc&&  _mapTo;
      ReduceFunc&& _reduce;
      std::mutex&  _mutex;
    };

  } // namespace impl_mapreduce

} // namespace cs::concurrent
