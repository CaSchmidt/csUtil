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

#pragma once

#include <cstdint>

#include <chrono>
#include <type_traits>

#include <cs/Core/csutil_config.h>

namespace cs {

  ////// std::chrono Helpers /////////////////////////////////////////////////

  template<typename, typename = void>
  struct is_clock_to_sys : std::false_type {};

  template<typename ClockT>
  struct is_clock_to_sys<ClockT,std::void_t<decltype(ClockT::to_sys(typename ClockT::time_point{}))>> : std::true_type {};

  template<typename ClockT>
  inline constexpr bool is_clock_to_sys_v = is_clock_to_sys<ClockT>::value;

  template<typename ClockT>
  inline std::chrono::system_clock::time_point toSystemTime(const typename ClockT::tim_point& clktim,
                                                            std::enable_if_t<is_clock_to_sys_v<ClockT>> * = nullptr)
  {
    return ClockT::to_sys(clktim);
  }

  template<typename ClockT>
  inline std::chrono::system_clock::time_point toSystemTime(const typename ClockT::time_point& clktim,
                                                            std::enable_if_t<!is_clock_to_sys_v<ClockT>> * = nullptr)
  {
    const std::chrono::utc_clock::time_point utctim = ClockT::to_utc(clktim);
    return std::chrono::utc_clock::to_sys(utctim);
  }

  ////// Time Value class ////////////////////////////////////////////////////

  class CS_UTIL_EXPORT TimeVal {
  public:
    using microseconds = std::chrono::duration<int64_t,std::micro>;
    using   value_type = microseconds::rep;

    explicit TimeVal(const value_type value = 0) noexcept;

    explicit TimeVal(const std::chrono::seconds secs,
                     const std::chrono::microseconds usecs) noexcept;

    TimeVal(const TimeVal&) noexcept = default;
    TimeVal& operator=(const TimeVal&) noexcept = default;

    TimeVal(TimeVal&&) noexcept = default;
    TimeVal& operator=(TimeVal&&) noexcept = default;

    ~TimeVal() noexcept = default;

    TimeVal& operator-=(const TimeVal& other);
    TimeVal& operator+=(const TimeVal& other);

    static bool isValid(const std::chrono::seconds secs,
                        const std::chrono::microseconds usecs);
    bool isValid() const;

    std::chrono::seconds secs() const;
    std::chrono::microseconds usecs() const;

    value_type value() const;

  private:
    microseconds _value{};
  };

  inline TimeVal operator-(const TimeVal& a, const TimeVal& b)
  {
    TimeVal result(a);
    result -= b;
    return result;
  }

  inline TimeVal operator+(const TimeVal& a, const TimeVal& b)
  {
    TimeVal result(a);
    result += b;
    return result;
  }

  ////// Useful Time Functions ///////////////////////////////////////////////

  CS_UTIL_EXPORT void sleep(const unsigned int secs);

  CS_UTIL_EXPORT void msleep(const unsigned int millisecs);

  CS_UTIL_EXPORT void usleep(const unsigned int microsecs);

  CS_UTIL_EXPORT uint64_t tickCountMs();

  CS_UTIL_EXPORT uint64_t tickCountUs();

} // namespace cs
