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

  Logger::Logger(FILE *file, const bool is_owner)
    : _file(file)
    , _is_owner(is_owner)
  {
  }

  Logger::~Logger()
  {
    if( _file == stderr  ||  _file == stdin  ||  _file == stdout ) {
      return;
    }
    if( _is_owner ) {
      fclose(_file);
    }
  }

  void Logger::logFlush() const
  {
    fflush(_file);
  }

  constexpr auto MAX_STD_SIZE = maxab_v<std::u8string_view::size_type,int>;

  void Logger::logText(const std::u8string_view& sv) const
  {
    if( !sv.empty()  &&  sv.size() <= MAX_STD_SIZE ) {
      fprintf(_file, "%.*s\n", int(sv.size()), CSTR(sv.data()));
    }
  }

  void Logger::logWarning(const std::u8string_view& sv) const
  {
    if( !sv.empty()  &&  sv.size() <= MAX_STD_SIZE ) {
      fprintf(_file, "WARNING: %.*s\n", int(sv.size()), CSTR(sv.data()));
    }
  }

  void Logger::logWarning(const int lineno, const std::u8string_view& sv) const
  {
    if( !sv.empty()  &&  sv.size() <= MAX_STD_SIZE ) {
      fprintf(_file, "WARNING:%d: %.*s\n", lineno, int(sv.size()), CSTR(sv.data()));
    }
  }

  void Logger::logError(const std::u8string_view& sv) const
  {
    if( !sv.empty()  &&  sv.size() <= MAX_STD_SIZE ) {
      fprintf(_file, "ERROR: %.*s\n", int(sv.size()), CSTR(sv.data()));
    }
  }

  void Logger::logError(const int lineno, const std::u8string_view& sv) const
  {
    if( !sv.empty()  &&  sv.size() <= MAX_STD_SIZE ) {
      fprintf(_file, "ERROR:%d: %.*s\n", lineno, int(sv.size()), CSTR(sv.data()));
    }
  }

} // namespace cs
