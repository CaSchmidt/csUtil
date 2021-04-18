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

#ifndef CSOUTPUTCONTEXT_H
#define CSOUTPUTCONTEXT_H

#include <string>

#include <csUtil/csutil_config.h>

class csILogger;
class csIProgress;

class CS_UTIL_EXPORT csOutputContext {
public:
  csOutputContext(const csILogger *logger, const bool logger_flush = false) noexcept;
  csOutputContext(const csIProgress *progress, const bool progress_flush = false) noexcept;
  csOutputContext(const csILogger *logger, const bool logger_flush,
                  const csIProgress *progress, const bool progress_flush) noexcept;
  ~csOutputContext() noexcept;

  void logFlush() const;

  void logText(const char8_t *text) const;
  void logText(const std::u8string& text) const;

  void logWarning(const char8_t *warning) const;
  void logWarning(const std::u8string& warning) const;
  void logWarning(const int line, const char8_t *warning) const;
  void logWarning(const int line, const std::u8string& warning) const;

  void logError(const char8_t *error) const;
  void logError(const std::u8string& error) const;
  void logError(const int line, const char8_t *error) const;
  void logError(const int line, const std::u8string& error) const;

  void progressFlush() const;

  void setProgressMaximum(const int max) const;
  void setProgressMinimum(const int min) const;
  void setProgressRange(const int min, const int max) const;
  void setProgressValue(const int val) const;

private:
  csOutputContext(const csOutputContext&) noexcept = delete;
  csOutputContext& operator=(const csOutputContext&) noexcept = delete;

  csOutputContext(csOutputContext&&) noexcept = delete;
  csOutputContext& operator=(csOutputContext&&) noexcept = delete;

  const csILogger *_logger{nullptr};
  bool _logger_flush{false};
  const csIProgress *_progress{nullptr};
  bool _progress_flush{false};
};

#endif // CSOUTPUTCONTEXT_H
