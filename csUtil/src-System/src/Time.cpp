/****************************************************************************
** Copyright (c) 2021, Carsten Schmidt. All rights reserved.
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

#include <chrono>
#include <thread>

#include "cs/System/Time.h"

namespace cs {

  namespace chr = std::chrono;

  ////// Private /////////////////////////////////////////////////////////////

  namespace impl_time {

    template<typename Clock, typename Duration>
    inline uint64_t getTickCount()
    {
      const Duration now = chr::duration_cast<Duration>(Clock::now().time_since_epoch());

      return static_cast<uint64_t>(now.count());
    }

  } // namespace impl_time

  ////// TimeVal - public ////////////////////////////////////////////////////

  TimeVal::TimeVal(const value_type value) noexcept
    : _value{value}
  {
  }

  TimeVal::TimeVal(const std::chrono::seconds secs,
                   const std::chrono::microseconds usecs) noexcept
    : _value{microseconds::zero()}
  {
    if( isValid(secs, usecs) ) {
      _value += chr::duration_cast<microseconds>(secs);
      _value += chr::duration_cast<microseconds>(usecs);
    }
  }

  TimeVal& TimeVal::operator-=(const TimeVal& other)
  {
    _value -= other._value;
    return *this;
  }

  TimeVal& TimeVal::operator+=(const TimeVal& other)
  {
    _value += other._value;
    return *this;
  }

  bool TimeVal::isValid(const std::chrono::seconds secs,
                        const std::chrono::microseconds usecs)
  {
    constexpr auto    ZERO_s  = chr::seconds::zero();
    constexpr auto    ZERO_us = chr::microseconds::zero();
    constexpr auto MILLION_us = chr::microseconds{1000000};

    return secs >= ZERO_s  &&  usecs >= ZERO_us  &&  usecs < MILLION_us;
  }

  bool TimeVal::isValid() const
  {
    return _value >= microseconds::zero();
  }

  std::chrono::seconds TimeVal::secs() const
  {
    return chr::duration_cast<chr::seconds>(_value);
  }

  std::chrono::microseconds TimeVal::usecs() const
  {
    const chr::seconds secs = chr::duration_cast<chr::seconds>(_value);

    return chr::duration_cast<chr::microseconds>(_value) - chr::duration_cast<chr::microseconds>(secs);
  }

  TimeVal::value_type TimeVal::value() const
  {
    return _value.count();
  }

  ////// Public //////////////////////////////////////////////////////////////

  CS_UTIL_EXPORT void sleep(const unsigned int secs)
  {
    std::this_thread::sleep_for(chr::seconds(secs));
  }

  CS_UTIL_EXPORT void msleep(const unsigned int millisecs)
  {
    std::this_thread::sleep_for(chr::milliseconds(millisecs));
  }

  CS_UTIL_EXPORT void usleep(const unsigned int microsecs)
  {
    std::this_thread::sleep_for(chr::microseconds(microsecs));
  }

  CS_UTIL_EXPORT uint64_t tickCountMs()
  {
    using    Clock = chr::steady_clock;
    using Duration = chr::milliseconds;

    return impl_time::getTickCount<Clock,Duration>();
  }

  CS_UTIL_EXPORT uint64_t tickCountUs()
  {
    using    Clock = chr::steady_clock;
    using Duration = chr::microseconds;

    return impl_time::getTickCount<Clock,Duration>();
  }

} // namespace cs
