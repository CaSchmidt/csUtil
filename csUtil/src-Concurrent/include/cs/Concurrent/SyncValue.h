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

#include <atomic>

namespace cs {

  template<typename T>
  class SyncValue {
  public:
    using atomic_type = std::atomic<T>;
    using  value_type = typename atomic_type::value_type;

    SyncValue(const value_type desired = value_type{}) noexcept
      : _atomic(desired)
    {
    }

    ~SyncValue() noexcept
    {
    }

    inline void notify_all(const value_type desired,
                           const std::memory_order order = std::memory_order_seq_cst)
    {
      store(desired, order);
      _atomic.notify_all();
    }

    inline void notify_one(const value_type desired,
                           const std::memory_order order = std::memory_order_seq_cst)
    {
      store(desired, order);
      _atomic.notify_one();
    }

    inline void store(const value_type desired,
                      const std::memory_order order = std::memory_order_seq_cst)
    {
      _atomic.store(desired, order);
    }

    inline void wait(const value_type desired,
                     const std::memory_order order_cmpxchg = std::memory_order_seq_cst,
                     const std::memory_order order_wait = std::memory_order_seq_cst)
    {
      for(value_type expected = desired;
          !_atomic.compare_exchange_weak(expected, desired, order_cmpxchg);
          expected = desired) {
        _atomic.wait(expected, order_wait);
      }
    }

  private:
    atomic_type _atomic;
  };

} // namespace cs
