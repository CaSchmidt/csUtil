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

#include "cs/Logging/DualLogger.h"

namespace cs {

  ////// public //////////////////////////////////////////////////////////////

  DualLogger::DualLogger(const LoggerPtr& a, const LoggerPtr& b) noexcept
    : _a(a)
    , _b(b)
  {
  }

  DualLogger::~DualLogger() noexcept
  {
  }

  void DualLogger::logFlush() const
  {
    if( _a ) {
      _a->logFlush();
    }
    if( _b ) {
      _b->logFlush();
    }
  }

  void DualLogger::logText(const std::u8string_view& msg) const
  {
    if( _a ) {
      _a->logText(msg);
    }
    if( _b ) {
      _b->logText(msg);
    }
  }

  void DualLogger::logWarning(const std::u8string_view& msg) const
  {
    if( _a ) {
      _a->logWarning(msg);
    }
    if( _b ) {
      _b->logWarning(msg);
    }
  }

  void DualLogger::logWarning(const std::size_t lineno, const std::u8string_view& msg) const
  {
    if( _a ) {
      _a->logWarning(lineno, msg);
    }
    if( _b ) {
      _b->logWarning(lineno, msg);
    }
  }

  void DualLogger::logError(const std::u8string_view& msg) const
  {
    if( _a ) {
      _a->logError(msg);
    }
    if( _b ) {
      _b->logError(msg);
    }
  }

  void DualLogger::logError(const std::size_t lineno, const std::u8string_view& msg) const
  {
    if( _a ) {
      _a->logError(lineno, msg);
    }
    if( _b ) {
      _b->logError(lineno, msg);
    }
  }

} // namespace cs
