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

#include "cs/Concurrent/ThreadPool.h"

namespace cs {

  ////// Macros //////////////////////////////////////////////////////////////

#undef HAVE_WORKER_DEBUG

  ////// public //////////////////////////////////////////////////////////////

  ThreadPool::ThreadPool(const size_type count,
                         const bool finish_on_dtor) noexcept
    : _finish_on_dtor{finish_on_dtor}
  {
    start(count);
  }

  ThreadPool::~ThreadPool() noexcept
  {
    if( _finish_on_dtor ) {
      finish();
    } else {
      cancel();
    }

    clear();
  }

  bool ThreadPool::isValid() const
  {
    return !_pool.empty();
  }

  bool ThreadPool::start(const size_type count)
  {
    if( isValid() ) {
      return false;
    }

    clear();

    for(size_type i = 0; i < count; i++) {
      _pool.push_back(std::thread(&ThreadPool::worker, this, i));
    }

    return isValid();
  }

  void ThreadPool::cancel(bool *is_empty)
  {
    join(Cancel);

    if( is_empty != nullptr ) {
      *is_empty = _queue.empty();
    }
  }

  void ThreadPool::finish()
  {
    join(Finish);
  }

  void ThreadPool::clear(bool *is_empty)
  {
    { // lock
      std::lock_guard<std::mutex> guard(_mutex);

      if( is_empty != nullptr ) {
        *is_empty = _queue.empty();
      }

      while( !_queue.empty() ) {
        _queue.pop();
      }
    } // unlock
    _cv.notify_all(); // why?
  }

  bool ThreadPool::dispatch(Runnable runnable)
  {
    if( !isValid()  ||  !runnable ) {
      return false;
    }

    { // lock
      std::lock_guard<std::mutex> guard(_mutex);
      _queue.push(std::move(runnable));
    } // unlock
    _cv.notify_one();

    return true;
  }

  ////// private /////////////////////////////////////////////////////////////

  void ThreadPool::join(const PoolState reason)
  {
    { // lock
      std::lock_guard<std::mutex> guard(_mutex);
      _state = reason;
    } // unlock
    _cv.notify_all();

    for(std::thread& t : _pool) {
      t.join();
    }
    _pool.clear();
  }

  void ThreadPool::worker(const size_type id)
  {
#ifdef HAVE_WORKER_DEBUG
    fprintf(stderr, "Starting %d...\n", int(id));
    fflush(stderr);
#else
    static_cast<void>(id); // silence warning of unused 'id'
#endif

    while( true ) {
      Runnable runnable;

      { // lock
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [this]() -> bool {
          return _state != Idle  ||  !_queue.empty();
        });

        if( _state == Cancel ) {
          break;
        } else if( _state == Finish  &&  _queue.empty() ) {
          break;
        } else if( !_queue.empty() ) {
          runnable = std::move(_queue.front());
          _queue.pop();
        }
      } // unlock

      if( runnable ) {
        runnable();
      }
    } // while( true )

#ifdef HAVE_WORKER_DEBUG
    fprintf(stderr, "Exiting %d...\n", int(id));
    fflush(stderr);
#endif
  }

} // namespace cs
