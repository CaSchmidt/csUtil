/****************************************************************************
** Copyright (c) 2020, Carsten Schmidt. All rights reserved.
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

#ifndef CSWLOGGER_H
#define CSWLOGGER_H

#include <QtWidgets/QTextBrowser>

#include <csUtil/csILogger.h>

class CS_UTIL_EXPORT csWLogger
    : public QTextBrowser
    , public csILogger {
  Q_OBJECT
public:
  csWLogger(QWidget *parent = nullptr);
  ~csWLogger();

  void logFlush() const;

  void logText(const char *) const;
  void logText(const std::string&) const;

  void logWarning(const char *) const;
  void logWarning(const std::string&) const;
  void logWarning(const int, const char *) const;
  void logWarning(const int, const std::string&) const;

  void logError(const char *) const;
  void logError(const std::string&) const;
  void logError(const int, const char *) const;
  void logError(const int, const std::string&) const;

private slots:
  void impl_logText(const QString& s);
  void impl_logWarning(const QString& s);
  void impl_logError(const QString& s);
};

#endif // CSWLOGGER_H
