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

#include <functional>
#include <type_traits>
#include <utility>

namespace cs {

  namespace meta {

    ////// Implementation ////////////////////////////////////////////////////

    namespace impl_meta {

      using size_t = std::size_t;

      // accumulate() ////////////////////////////////////////////////////////

      template<typename Ret, typename Call, typename ...Args>
      concept IsAccumulate = std::is_invocable_r_v<Ret,Call,Ret,size_t,Args...>;

      template<typename T, size_t I, typename Call, typename ...Args>
      requires IsAccumulate<T,Call,Args...>
      inline T InvokeAccumulate(T init, Call call, Args&&... args)
      {
        // Syntax: T call(T init, const size_t I, ...)
        return std::invoke(call, std::move(init), I, std::forward<Args>(args)...);
      }

      template<typename T, size_t N, size_t CNT>
      struct Accumulate {
        static_assert(N > 0  &&  0 < CNT  &&  CNT <= N);

        template<typename Call, typename ...Args>
        inline static T run(T init, Call call, Args&&... args)
        {
          constexpr size_t I = N - CNT; // CNT := [N, ..., 1]
          init = InvokeAccumulate<T,I>(std::move(init), call, std::forward<Args>(args)...);
          return Accumulate<T,N,CNT-1>::run(std::move(init), call, std::forward<Args>(args)...);
        }
      };

      template<typename T, size_t N>
      struct Accumulate<T,N,0> {
        static_assert(N > 0);

        template<typename Call, typename ...Args>
        inline static T run(T init, Call /*call*/, Args&&... /*args*/)
        {
          return init;
        }
      };

      // for_each() //////////////////////////////////////////////////////////

      template<typename Call, typename ...Args>
      concept IsForEach = std::is_invocable_v<Call,size_t,Args...>;

      template<size_t I, typename Call, typename ...Args>
      requires IsForEach<Call,Args...>
      inline void InvokeForEach(Call call, Args&&... args)
      {
        // Syntax: void call(const size_t I, ...)
        std::invoke(call, I, std::forward<Args>(args)...);
      }

      template<size_t N, size_t CNT>
      struct ForEach {
        static_assert(N > 0  &&  0 < CNT  &&  CNT <= N);

        template<typename Call, typename ...Args>
        inline static void run(Call call, Args&&... args)
        {
          constexpr size_t I = N - CNT; // CNT := [N, ..., 1]
          InvokeForEach<I>(call, std::forward<Args>(args)...);
          ForEach<N,CNT-1>::run(call, std::forward<Args>(args)...);
        }
      };

      template<size_t N>
      struct ForEach<N,0> {
        static_assert(N > 0);

        template<typename Call, typename ...Args>
        inline static void run(Call /*call*/, Args&&... /*args*/)
        {
        }
      };

    } // namespace impl_meta

    ////// User Interface ////////////////////////////////////////////////////

    template<typename T, std::size_t N, typename Call, typename ...Args>
    inline T accumulate(T init, Call call, Args&&... args)
    {
      return impl_meta::Accumulate<T,N,N>::run(std::move(init), call, std::forward<Args>(args)...);
    }

    template<std::size_t N, typename Call, typename ...Args>
    inline void for_each(Call call, Args&&... args)
    {
      impl_meta::ForEach<N,N>::run(call, std::forward<Args>(args)...);
    }

    ////// Algorithms ////////////////////////////////////////////////////////

    template<std::size_t N, typename T>
    inline void copy(T *dest, const T *src)
    {
      for_each<N>([](const std::size_t I, T *dest, const T *src) -> void {
        dest[I] = src[I];
      }, dest, src);
    }

    template<std::size_t N, typename T>
    inline T dot(const T *a, const T *b, T sum = T())
    {
      return accumulate<T,N>(std::move(sum),
                             [](T sum, const std::size_t I, const T *a, const T *b) -> T {
        return sum + a[I]*b[I];
      }, a, b);
    }

    template<std::size_t N, typename T>
    inline void fill(T *dest, const T& value = T())
    {
      for_each<N>([](const std::size_t I, T *dest, const T& value) -> void {
        dest[I] = value;
      }, dest, value);
    }

  } // namespace meta

} // namespace cs
