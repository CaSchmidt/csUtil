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

#include <QtCore/QCoreApplication>
#include <QtCore/QMetaObject>

#include <csUtil/csWLogger.h>

#include <csUtil/csQStringUtil.h>

////// Private ///////////////////////////////////////////////////////////////

namespace priv {

  void invokeLogText(QObject *o, const QString& s)
  {
    QMetaObject::invokeMethod(o, "impl_logText", Qt::AutoConnection, Q_ARG(QString, s));
  }

  void invokeLogWarning(QObject *o, const QString& s)
  {
    QMetaObject::invokeMethod(o, "impl_logWarning", Qt::AutoConnection, Q_ARG(QString, s));
  }

  void invokeLogError(QObject *o, const QString& s)
  {
    QMetaObject::invokeMethod(o, "impl_logError", Qt::AutoConnection, Q_ARG(QString, s));
  }

} // namespace priv

////// public ////////////////////////////////////////////////////////////////

csWLogger::csWLogger(QWidget *parent)
  : QTextBrowser(parent)
  , csILogger()
{
  setReadOnly(true);
}

csWLogger::~csWLogger()
{
}

void csWLogger::logFlush() const
{
  QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

void csWLogger::logText(const char8_t *text) const
{
  const QString s = cs::toQString(text);
  priv::invokeLogText(const_cast<csWLogger*>(this), s);
}

void csWLogger::logWarning(const char8_t *warning) const
{
  const QString s = tr("WARNING: %1").arg(cs::toQString(warning));
  priv::invokeLogWarning(const_cast<csWLogger*>(this), s);
}

void csWLogger::logWarning(const int lineno, const char8_t *warning) const
{
  const QString s = tr("WARNING:%1: %2").arg(lineno).arg(cs::toQString(warning));
  priv::invokeLogWarning(const_cast<csWLogger*>(this), s);
}

void csWLogger::logError(const char8_t *error) const
{
  const QString s = tr("ERROR: %1").arg(cs::toQString(error));
  priv::invokeLogError(const_cast<csWLogger*>(this), s);
}

void csWLogger::logError(const int lineno, const char8_t *error) const
{
  const QString s = tr("ERROR:%1: %2").arg(lineno).arg(cs::toQString(error));
  priv::invokeLogError(const_cast<csWLogger*>(this), s);
}

////// private slots /////////////////////////////////////////////////////////

void csWLogger::impl_logText(const QString& s)
{
  setTextColor(Qt::black);
  append(s);
}

void csWLogger::impl_logWarning(const QString& s)
{
  setTextColor(Qt::blue);
  append(s);
}

void csWLogger::impl_logError(const QString& s)
{
  setTextColor(Qt::red);
  append(s);
}
