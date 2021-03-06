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

#ifndef CSWPROGRESSLOGGER_H
#define CSWPROGRESSLOGGER_H

#include <memory>

#include <QtCore/QFutureWatcher>
#include <QtWidgets/QDialog>
#include <QtWidgets/QProgressBar>

#include <csUtil/csutil_config.h>

class csILogger;
class csIProgress;

namespace priv {
  class IProgressImpl;
} // namespace priv

namespace Ui {
  class csWProgressLogger;
} // namespace Ui

class CS_UTIL_EXPORT csWProgressLogger : public QDialog {
  Q_OBJECT
public:
  csWProgressLogger(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
  ~csWProgressLogger();

  const csILogger *logger() const;
  const csIProgress *progress() const;

  template<typename T>
  void setFutureWatcher(QFutureWatcher<T> *watcher);

public slots:
  void stepValue(int dir = 1);

private slots:
  void finish();
  void monitorProgress(int value);

private:  
  QProgressBar *progressBar();

  Ui::csWProgressLogger *ui{nullptr};

  std::unique_ptr<priv::IProgressImpl> _progress{nullptr};

signals:
  void canceled();
  void valueChanged(int value);
};

////// Implementation ////////////////////////////////////////////////////////

template<typename T>
void csWProgressLogger::setFutureWatcher(QFutureWatcher<T> *watcher)
{
  if( watcher == nullptr ) {
    return;
  }

  disconnect(this, &csWProgressLogger::valueChanged,
             this, &csWProgressLogger::monitorProgress);

  connect(this, &csWProgressLogger::canceled,
          watcher, &QFutureWatcher<T>::cancel);

  connect(watcher, &QFutureWatcher<T>::finished,
          this, &csWProgressLogger::finish);
  connect(watcher, &QFutureWatcher<T>::progressRangeChanged,
          progressBar(), &QProgressBar::setRange);
  connect(watcher, &QFutureWatcher<T>::progressValueChanged,
          progressBar(), &QProgressBar::setValue);
}

#endif // CSWPROGRESSLOGGER_H
