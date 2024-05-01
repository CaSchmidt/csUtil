/****************************************************************************
** Copyright (c) 2018, Carsten Schmidt. All rights reserved.
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

#include <cstddef>

#include <memory>
#include <string_view>

#include <cs/Core/csutil_config.h>

namespace cs {

  using LoggerPtr = std::shared_ptr<class AbstractLogger>;

  class CS_UTIL_EXPORT AbstractLogger {
  public:
    virtual ~AbstractLogger() noexcept;

    virtual void logFlush() const;

    virtual void logText(const std::u8string_view& sv) const = 0;

    virtual void logWarning(const std::u8string_view& sv) const = 0;
    virtual void logWarning(const std::size_t lineno, const std::u8string_view& sv) const = 0;

    virtual void logError(const std::u8string_view& sv) const = 0;
    virtual void logError(const std::size_t lineno, const std::u8string_view& sv) const = 0;

  protected:
    AbstractLogger() noexcept;

  private:
    AbstractLogger(const AbstractLogger&) noexcept = delete;
    AbstractLogger& operator=(const AbstractLogger&) noexcept = delete;
    AbstractLogger(AbstractLogger&&) noexcept = delete;
    AbstractLogger& operator=(AbstractLogger&&) noexcept = delete;
  };

  struct CS_UTIL_EXPORT NoDeleteLoggerPtr {
    void operator()(AbstractLogger*) const;
  };

} // namespace cs
