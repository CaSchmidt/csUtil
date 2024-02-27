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

namespace cs {

  template<typename T>
  class SyncCondition {
  public:
    using value_type = T;

    SyncCondition(const value_type desired = value_type{}) noexcept
      : _value{desired}
    {
    }

    ~SyncCondition() noexcept
    {
    }

    SyncCondition& operator=(const value_type desired)
    {
      std::lock_guard<std::mutex> guard(_mutex);
      _value = desired;

      return *this;
    }

    inline void notify_all()
    {
      _cv.notify_all();
    }

    void notify_all(const value_type desired)
    {
      operator=(desired);
      notify_all();
    }

    inline void notify_one()
    {
      _cv.notify_one();
    }

    void notify_one(const value_type desired)
    {
      operator=(desired);
      notify_one();
    }

    void wait(const value_type desired)
    {
      std::unique_lock<std::mutex> lock(_mutex);
      _cv.wait(lock, [this,desired]() -> bool {
        return _value == desired;
      });
    }

  private:
    std::condition_variable _cv;
    std::mutex _mutex;
    value_type _value;
  };

} // namespace cs
