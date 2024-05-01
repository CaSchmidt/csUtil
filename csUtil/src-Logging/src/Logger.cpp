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

#include <algorithm>

#include "cs/Logging/Logger.h"

#include "cs/Core/TypeTraits.h"

namespace cs {

  namespace impl_logger {

    constexpr auto MAX_INT_SIZE = maxab_v<std::size_t,int>;

    inline bool isWidthSize(const std::u8string_view& view)
    {
      return !view.empty()  &&  view.size() <= MAX_INT_SIZE;
    }

    inline int toInt(const std::size_t in)
    {
      return static_cast<int>(std::min(in, MAX_INT_SIZE));
    }

  } // namespace impl_logger

  ////// public //////////////////////////////////////////////////////////////

  Logger::Logger(FILE *file, const bool is_owner, const ctor_tag&)
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

  void Logger::logText(const std::u8string_view& msg) const
  {
    if( impl_logger::isWidthSize(msg) ) {
      fprintf(_file, "%.*s\n",
              int(msg.size()), CSTR(msg.data()));
    }
  }

  void Logger::logWarning(const std::u8string_view& msg) const
  {
    if( impl_logger::isWidthSize(msg) ) {
      fprintf(_file, "WARNING: %.*s\n",
              int(msg.size()), CSTR(msg.data()));
    }
  }

  void Logger::logWarning(const std::size_t lineno, const std::u8string_view& msg) const
  {
    if( impl_logger::isWidthSize(msg) ) {
      fprintf(_file, "WARNING(L%d): %.*s\n",
              impl_logger::toInt(lineno), int(msg.size()), CSTR(msg.data()));
    }
  }

  void Logger::logError(const std::u8string_view& msg) const
  {
    if( impl_logger::isWidthSize(msg) ) {
      fprintf(_file, "ERROR: %.*s\n",
              int(msg.size()), CSTR(msg.data()));
    }
  }

  void Logger::logError(const std::size_t lineno, const std::u8string_view& msg) const
  {
    if( impl_logger::isWidthSize(msg) ) {
      fprintf(_file, "ERROR(L%d): %.*s\n",
              impl_logger::toInt(lineno), int(msg.size()), CSTR(msg.data()));
    }
  }

  LoggerPtr Logger::make(FILE *file, const bool is_owner)
  {
    return std::make_shared<Logger>(file, is_owner);
  }

} // namespace cs
