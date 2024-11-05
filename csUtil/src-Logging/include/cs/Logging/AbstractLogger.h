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

#include <format>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include <cs/Core/csutil_config.h>

namespace cs {

  using LoggerPtr = std::shared_ptr<class AbstractLogger>;

  class CS_UTIL_EXPORT AbstractLogger {
  public:
    virtual ~AbstractLogger() noexcept;

    virtual void logFlush() const;

    virtual void logText(const std::u8string_view& msg) const = 0;

    virtual void logWarning(const std::u8string_view& msg) const = 0;
    virtual void logWarning(const std::size_t lineno, const std::u8string_view& msg) const = 0;

    virtual void logError(const std::u8string_view& msg) const = 0;
    virtual void logError(const std::size_t lineno, const std::u8string_view& msg) const = 0;

    // Formatted Logging /////////////////////////////////////////////////////

    template<typename Arg1, typename ...ArgN>
    inline void logText(const std::u8string_view& fmt,
                        Arg1&& arg1, ArgN&&... argn)
    {
      const std::string msg = format(fmt,
                                     std::forward<Arg1>(arg1),
                                     std::forward<ArgN>(argn)...);
      logText(toUtf8View(msg));
    }

    template<typename Arg1, typename ...ArgN>
    inline void logWarning(const std::u8string_view& fmt,
                           Arg1&& arg1, ArgN&&... argn)
    {
      const std::string msg = format(fmt,
                                     std::forward<Arg1>(arg1),
                                     std::forward<ArgN>(argn)...);
      logWarning(toUtf8View(msg));
    }

    template<typename Arg1, typename ...ArgN>
    inline void logWarning(const std::size_t lineno, const std::u8string_view& fmt,
                           Arg1&& arg1, ArgN&&... argn)
    {
      const std::string msg = format(fmt,
                                     std::forward<Arg1>(arg1),
                                     std::forward<ArgN>(argn)...);
      logWarning(lineno, toUtf8View(msg));
    }

    template<typename Arg1, typename ...ArgN>
    inline void logError(const std::u8string_view& fmt,
                         Arg1&& arg1, ArgN&&... argn)
    {
      const std::string msg = format(fmt,
                                     std::forward<Arg1>(arg1),
                                     std::forward<ArgN>(argn)...);
      logError(toUtf8View(msg));
    }

    template<typename Arg1, typename ...ArgN>
    inline void logError(const std::size_t lineno, const std::u8string_view& fmt,
                         Arg1&& arg1, ArgN&&... argn)
    {
      const std::string msg = format(fmt,
                                     std::forward<Arg1>(arg1),
                                     std::forward<ArgN>(argn)...);
      logError(lineno, toUtf8View(msg));
    }

  protected:
    AbstractLogger() noexcept;

    template<typename Arg1, typename ...ArgN>
    inline std::string format(const std::u8string_view& fmt,
                              Arg1&& arg1, ArgN&&... argn) const
    {
      return std::vformat(toView(fmt),
                          std::make_format_args(std::forward<Arg1>(arg1),
                                                std::forward<ArgN>(argn)...));
    }

    constexpr std::string_view toView(const std::u8string_view& view) const
    {
      return !view.empty()
          ? std::string_view(reinterpret_cast<const char*>(view.data()), view.size())
          : std::string_view();
    }

    constexpr std::u8string_view toUtf8View(const std::string_view& view) const
    {
      return !view.empty()
          ? std::u8string_view(reinterpret_cast<const char8_t*>(view.data()), view.size())
          : std::u8string_view();
    }

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
