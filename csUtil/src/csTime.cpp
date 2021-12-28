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

#include "csUtil/csTime.h"

////// Private ///////////////////////////////////////////////////////////////

namespace impl {

  template<typename Clock, typename Duration>
  inline uint64_t getTickCount()
  {
    const Duration now = std::chrono::duration_cast<Duration>(Clock::now().time_since_epoch());

    return static_cast<uint64_t>(now.count());
  }

} // namespace impl

////// Public ////////////////////////////////////////////////////////////////

CS_UTIL_EXPORT void csSleep(const unsigned int secs)
{
  std::this_thread::sleep_for(std::chrono::seconds(secs));
}

CS_UTIL_EXPORT void csMSleep(const unsigned int millisecs)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(millisecs));
}

CS_UTIL_EXPORT void csUSleep(const unsigned int microsecs)
{
  std::this_thread::sleep_for(std::chrono::microseconds(microsecs));
}

CS_UTIL_EXPORT uint64_t csTickCountMs()
{
  using    Clock = std::chrono::steady_clock;
  using Duration = std::chrono::milliseconds;

  return impl::getTickCount<Clock,Duration>();
}

CS_UTIL_EXPORT uint64_t csTickCountUs()
{
  using    Clock = std::chrono::steady_clock;
  using Duration = std::chrono::microseconds;

  return impl::getTickCount<Clock,Duration>();
}
