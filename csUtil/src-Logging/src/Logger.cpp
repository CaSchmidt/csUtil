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

#include "cs/Logging/Logger.h"

#include "cs/Core/TypeTraits.h"

namespace cs {

  ////// public //////////////////////////////////////////////////////////////

  Logger::Logger(FILE *file, const bool owner)
    : _file(file)
    , _owner(owner)
  {
  }

  Logger::~Logger()
  {
    if( _owner  &&  _file != stderr  &&  _file != stdout ) {
      fclose(_file);
    }
  }

  void Logger::logFlush() const
  {
    fflush(_file);
  }

  void Logger::logText(const char8_t *s) const
  {
    fprintf(_file, "%s\n", cs::CSTR(s));
  }

  void Logger::logWarning(const char8_t *s) const
  {
    fprintf(_file, "WARNING: %s\n", cs::CSTR(s));
  }

  void Logger::logWarning(const int lineno, const char8_t *s) const
  {
    fprintf(_file, "WARNING:%d: %s\n", lineno, cs::CSTR(s));
  }

  void Logger::logError(const char8_t *s) const
  {
    fprintf(_file, "ERROR: %s\n", cs::CSTR(s));
  }

  void Logger::logError(const int lineno, const char8_t *s) const
  {
    fprintf(_file, "ERROR:%d: %s\n", lineno, cs::CSTR(s));
  }

} // namespace cs