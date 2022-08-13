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

csOutputContext::csOutputContext(const csILogger *logger, const bool logger_flush) noexcept
  : csOutputContext(logger, logger_flush, nullptr, false)
{
}

csOutputContext::csOutputContext(const csIProgress *progress, const bool progress_flush) noexcept
  : csOutputContext(nullptr, false, progress, progress_flush)
{
}

csOutputContext::csOutputContext(const csILogger *logger, const bool logger_flush,
                                 const csIProgress *progress, const bool progress_flush) noexcept
  : _logger{logger}
  , _logger_flush{logger_flush}
  , _progress{progress}
  , _progress_flush{progress_flush}
{
}

csOutputContext::~csOutputContext() noexcept
{
}

void csOutputContext::logFlush() const
{
  if( _logger != nullptr ) {
    _logger->logFlush();
  }
}

void csOutputContext::logText(const char8_t *text) const
{
  if( _logger != nullptr ) {
    _logger->logText(text);
    if( _logger_flush ) {
      _logger->logFlush();
    }
  }
}

void csOutputContext::logText(const std::u8string& text) const
{
  if( _logger != nullptr ) {
    _logger->logText(text);
    if( _logger_flush ) {
      _logger->logFlush();
    }
  }
}

void csOutputContext::logWarning(const char8_t *warning) const
{
  if( _logger != nullptr ) {
    _logger->logWarning(warning);
    if( _logger_flush ) {
      _logger->logFlush();
    }
  }
}

void csOutputContext::logWarning(const std::u8string& warning) const
{
  if( _logger != nullptr ) {
    _logger->logWarning(warning);
    if( _logger_flush ) {
      _logger->logFlush();
    }
  }
}

void csOutputContext::logWarning(const int line, const char8_t *warning) const
{
  if( _logger != nullptr ) {
    _logger->logWarning(line, warning);
    if( _logger_flush ) {
      _logger->logFlush();
    }
  }
}

void csOutputContext::logWarning(const int line, const std::u8string& warning) const
{
  if( _logger != nullptr ) {
    _logger->logWarning(line, warning);
    if( _logger_flush ) {
      _logger->logFlush();
    }
  }
}

void csOutputContext::logError(const char8_t *error) const
{
  if( _logger != nullptr ) {
    _logger->logError(error);
    if( _logger_flush ) {
      _logger->logFlush();
    }
  }
}

void csOutputContext::logError(const std::u8string& error) const
{
  if( _logger != nullptr ) {
    _logger->logError(error);
    if( _logger_flush ) {
      _logger->logFlush();
    }
  }
}

void csOutputContext::logError(const int line, const char8_t *error) const
{
  if( _logger != nullptr ) {
    _logger->logError(line, error);
    if( _logger_flush ) {
      _logger->logFlush();
    }
  }
}

void csOutputContext::logError(const int line, const std::u8string& error) const
{
  if( _logger != nullptr ) {
    _logger->logError(line, error);
    if( _logger_flush ) {
      _logger->logFlush();
    }
  }
}

void csOutputContext::progressFlush() const
{
  if( _progress != nullptr ) {
    _progress->progressFlush();
  }
}

void csOutputContext::setProgressMaximum(const int max) const
{
  if( _progress != nullptr ) {
    _progress->setProgressMaximum(max);
    if( _progress_flush ) {
      _progress->progressFlush();
    }
  }
}

void csOutputContext::setProgressMinimum(const int min) const
{
  if( _progress != nullptr ) {
    _progress->setProgressMinimum(min);
    if( _progress_flush ) {
      _progress->progressFlush();
    }
  }
}

void csOutputContext::setProgressRange(const int min, const int max) const
{
  if( _progress != nullptr ) {
    _progress->setProgressRange(min, max);
    if( _progress_flush ) {
      _progress->progressFlush();
    }
  }
}

void csOutputContext::setProgressValue(const int val) const
{
  if( _progress != nullptr ) {
    _progress->setProgressValue(val);
    if( _progress_flush ) {
      _progress->progressFlush();
    }
  }
}
