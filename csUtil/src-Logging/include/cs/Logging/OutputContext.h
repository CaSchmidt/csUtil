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

#include <string>

#include <cs/Logging/AbstractLogger.h>
#include <cs/Logging/AbstractProgress.h>

namespace cs {

  class CS_UTIL_EXPORT OutputContext
      : public AbstractLogger
      , public AbstractProgress {
  public:
    OutputContext(const LoggerPtr& logger, const bool logger_flush = false) noexcept;
    OutputContext(const ProgressPtr& progress, const bool progress_flush = false) noexcept;
    OutputContext(const LoggerPtr& logger, const bool logger_flush,
                  const ProgressPtr& progress, const bool progress_flush) noexcept;
    ~OutputContext() noexcept;

    LoggerPtr logger() const;

    void logFlush() const;

    void logText(const std::u8string_view& msg) const;

    void logWarning(const std::u8string_view& msg) const;
    void logWarning(const std::size_t lineno, const std::u8string_view& msg) const;

    void logError(const std::u8string_view& msg) const;
    void logError(const std::size_t lineno, const std::u8string_view& msg) const;

    ProgressPtr progress() const;

    void progressFlush() const;

    void resetProgress() const;

    void setProgressRange(const std::size_t min, const std::size_t max) const;
    void setProgressValue(const std::size_t val) const;

  private:
    LoggerPtr _logger{nullptr};
    bool _logger_flush{false};
    ProgressPtr _progress{nullptr};
    bool _progress_flush{false};
  };

} // namespace cs
