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

#ifndef CSILOGGER_H
#define CSILOGGER_H

#ifdef HAVE_STD_FORMAT
# include <format>
#endif
#include <string>

#include <csUtil/csutil_config.h>

#ifdef HAVE_STD_FORMAT
# include <csUtil/csTypeTraits.h>
#endif

class CS_UTIL_EXPORT csILogger {
public:
  csILogger() noexcept;
  virtual ~csILogger() noexcept;

  virtual void logFlush() const;

  virtual void logText(const char8_t *) const = 0;
  virtual void logText(const std::u8string&) const;

  virtual void logWarning(const char8_t *) const = 0;
  virtual void logWarning(const std::u8string&) const;
  virtual void logWarning(const int, const char8_t *) const = 0;
  virtual void logWarning(const int, const std::u8string&) const;

  virtual void logError(const char8_t *) const = 0;
  virtual void logError(const std::u8string&) const;
  virtual void logError(const int, const char8_t *) const = 0;
  virtual void logError(const int, const std::u8string&) const;

#ifdef HAVE_STD_FORMAT
public:
  template<typename... Args>
  inline void logTextf(const char8_t *fmt, Args&&... args) const
  {
    const std::string msg = vformat(fmt, args...);
    logText(cs::UTF8(msg.data()));
  }

  template<typename... Args>
  inline void logWarningf(const char8_t *fmt, Args&&... args) const
  {
    const std::string msg = vformat(fmt, args...);
    logWarning(cs::UTF8(msg.data()));
  }

  template<typename... Args>
  inline void logWarningf(const int lineno, const char8_t *fmt, Args&&... args) const
  {
    const std::string msg = vformat(fmt, args...);
    logWarning(lineno, cs::UTF8(msg.data()));
  }

  template<typename... Args>
  inline void logErrorf(const char8_t *fmt, Args&&... args) const
  {
    const std::string msg = vformat(fmt, args...);
    logError(cs::UTF8(msg.data()));
  }

  template<typename... Args>
  inline void logErrorf(const int lineno, const char8_t *fmt, Args&&... args) const
  {
    const std::string msg = vformat(fmt, args...);
    logError(lineno, cs::UTF8(msg.data()));
  }

private:
  template<typename... Args>
  inline static std::string vformat(const char8_t *fmt, Args&&... args)
  {
    return std::vformat(cs::CSTR(fmt), std::make_format_args(args...));
  }
#endif

private:
  csILogger(const csILogger&) noexcept = delete;
  csILogger& operator=(const csILogger&) noexcept = delete;
  csILogger(csILogger&&) noexcept = delete;
  csILogger& operator=(csILogger&&) noexcept = delete;
};

#endif // CSILOGGER_H
