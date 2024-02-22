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

#include <condition_variable>
#include <list>
#include <queue>
#include <thread>

#include <cs/Concurrent/Runnable.h>

namespace cs {

  class CS_UTIL_EXPORT ThreadPool {
  public:
    using size_type = std::size_t;

    ThreadPool(const size_type count = std::thread::hardware_concurrency(),
               const bool finish_on_dtor = false) noexcept;
    ~ThreadPool() noexcept;

    bool isValid() const;

    void clear(bool *is_empty = nullptr);

    void cancel(bool *is_empty = nullptr);
    void finish();

    void dispatch(RunnablePtr ptr);

    void wait();

  private:
    enum PoolState {
      Idle = 0, // waiting for work to be queued...
      Finish,   // finish queued work; then exit
      Cancel    // exit, discarding queued work
    };

    void join(const PoolState reason);
    void worker(const size_type id);

    std::condition_variable _cv;
    bool                    _finish_on_dtor{false};
    std::mutex              _mutex;
    std::list<std::thread>  _pool;
    std::queue<RunnablePtr> _queue;
    PoolState               _state{Idle};
  };

} // namespace cs
