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

#include "cs/Logging/OutputContext.h"

#include "cs/Logging/ILogger.h"
#include "cs/Logging/IProgress.h"

namespace cs {

  OutputContext::OutputContext(const ILogger *logger, const bool logger_flush) noexcept
    : OutputContext(logger, logger_flush, nullptr, false)
  {
  }

  OutputContext::OutputContext(const IProgress *progress, const bool progress_flush) noexcept
    : OutputContext(nullptr, false, progress, progress_flush)
  {
  }

  OutputContext::OutputContext(const ILogger *logger, const bool logger_flush,
                               const IProgress *progress, const bool progress_flush) noexcept
    : _logger{logger}
    , _logger_flush{logger_flush}
    , _progress{progress}
    , _progress_flush{progress_flush}
  {
  }

  OutputContext::~OutputContext() noexcept
  {
  }

  const ILogger *OutputContext::logger() const
  {
    return _logger;
  }

  void OutputContext::logFlush() const
  {
    if( _logger != nullptr ) {
      _logger->logFlush();
    }
  }

  void OutputContext::logText(const std::u8string_view& sv) const
  {
    if( _logger != nullptr ) {
      _logger->logText(sv);
      if( _logger_flush ) {
        _logger->logFlush();
      }
    }
  }

  void OutputContext::logWarning(const std::u8string_view& sv) const
  {
    if( _logger != nullptr ) {
      _logger->logWarning(sv);
      if( _logger_flush ) {
        _logger->logFlush();
      }
    }
  }

  void OutputContext::logWarning(const int lineno, const std::u8string_view& sv) const
  {
    if( _logger != nullptr ) {
      _logger->logWarning(lineno, sv);
      if( _logger_flush ) {
        _logger->logFlush();
      }
    }
  }

  void OutputContext::logError(const std::u8string_view& sv) const
  {
    if( _logger != nullptr ) {
      _logger->logError(sv);
      if( _logger_flush ) {
        _logger->logFlush();
      }
    }
  }

  void OutputContext::logError(const int lineno, const std::u8string_view& sv) const
  {
    if( _logger != nullptr ) {
      _logger->logError(lineno, sv);
      if( _logger_flush ) {
        _logger->logFlush();
      }
    }
  }

  const IProgress *OutputContext::progress() const
  {
    return _progress;
  }

  void OutputContext::progressFlush() const
  {
    if( _progress != nullptr ) {
      _progress->progressFlush();
    }
  }

  void OutputContext::resetProgress() const
  {
    if( _progress != nullptr ) {
      _progress->resetProgress();
      if( _progress_flush ) {
        _progress->progressFlush();
      }
    }
  }

  void OutputContext::setProgressRange(const int min, const int max) const
  {
    if( _progress != nullptr ) {
      _progress->setProgressRange(min, max);
      if( _progress_flush ) {
        _progress->progressFlush();
      }
    }
  }

  void OutputContext::setProgressValue(const int val) const
  {
    if( _progress != nullptr ) {
      _progress->setProgressValue(val);
      if( _progress_flush ) {
        _progress->progressFlush();
      }
    }
  }

} // namespace cs
