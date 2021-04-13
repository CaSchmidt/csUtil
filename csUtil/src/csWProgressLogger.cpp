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

#include <QtWidgets/QPushButton>

#include <csUtil/csWProgressLogger.h>
#include "ui_csWProgressLogger.h"

#include <csUtil/csIProgress.h>

////// Private ///////////////////////////////////////////////////////////////

namespace priv {

  class IProgressImpl : public csIProgress {
  public:
    IProgressImpl(QProgressBar *bar) noexcept
      : _bar{bar}
    {
    }

    ~IProgressImpl() noexcept
    {
    }

    void progressFlush() const
    {
      QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    void setProgressMaximum(const int max)
    {
      _bar->setMaximum(max);
    }

    void setProgressMinimum(const int min)
    {
      _bar->setMinimum(min);
    }

    void setProgressRange(const int min, const int max)
    {
      _bar->setRange(min, max);
    }

    void setProgressValue(const int val)
    {
      _bar->setValue(val);
    }

  private:
    IProgressImpl() noexcept = delete;

    QProgressBar *_bar;
  };

  bool removeAllButtons(QDialogButtonBox *box)
  {
    QList<QAbstractButton*> buttons = box->buttons();
    for(QAbstractButton *button : buttons) {
      box->removeButton(button);
      delete button;
    }
    return box->buttons().isEmpty();
  }

} // namespace priv

////// public ////////////////////////////////////////////////////////////////

csWProgressLogger::csWProgressLogger(QWidget *parent, Qt::WindowFlags f)
  : QDialog(parent, f)
  , ui{new Ui::csWProgressLogger}
{
  ui->setupUi(this);

  _progress = std::make_unique<priv::IProgressImpl>(ui->progressBar);

  // User Interface //////////////////////////////////////////////////////////

  setWindowFlag(Qt::WindowCloseButtonHint, false);
  setWindowFlag(Qt::WindowContextHelpButtonHint, false);

  priv::removeAllButtons(ui->buttonBox);

  ui->buttonBox->addButton(QDialogButtonBox::Cancel);
  QPushButton *cancel = ui->buttonBox->button(QDialogButtonBox::Cancel);
  if( cancel != nullptr ) {
    cancel->disconnect();

    connect(cancel, &QPushButton::clicked, this, &csWProgressLogger::canceled);

    connect(cancel, &QPushButton::clicked, this, &csWProgressLogger::rejected);
    connect(cancel, &QPushButton::clicked, this, &csWProgressLogger::reject);

    cancel->setAutoDefault(false);
    cancel->setDefault(false);
  }

  // Signals & Slots /////////////////////////////////////////////////////////

  connect(ui->progressBar, &QProgressBar::valueChanged,
          this, &csWProgressLogger::valueChanged);
  connect(this, &csWProgressLogger::valueChanged,
          this, &csWProgressLogger::monitorProgress);
}

csWProgressLogger::~csWProgressLogger()
{
  delete ui;
}

const csILogger *csWProgressLogger::logger() const
{
  return ui->logBrowser;
}

csIProgress *csWProgressLogger::progress()
{
  return _progress.get();
}

const csIProgress *csWProgressLogger::progress() const
{
  return _progress.get();
}

////// public slots //////////////////////////////////////////////////////////

void csWProgressLogger::stepValue(int dir)
{
  if( dir == 0 ) {
    return;
  }
  const int inc = dir >= 0
      ? 1
      : -1;
  const int min = ui->progressBar->minimum();
  const int val = ui->progressBar->value();
  const int max = ui->progressBar->maximum();
  ui->progressBar->setValue(qBound<int>(min, val + inc, max));
}

////// private slots /////////////////////////////////////////////////////////

void csWProgressLogger::finish()
{
  priv::removeAllButtons(ui->buttonBox);

  ui->buttonBox->addButton(QDialogButtonBox::Close);
  QPushButton *close = ui->buttonBox->button(QDialogButtonBox::Close);
  if( close != nullptr ) {
    close->disconnect();

    connect(close, &QPushButton::clicked, this, &csWProgressLogger::accepted);
    connect(close, &QPushButton::clicked, this, &csWProgressLogger::accept);

    close->setAutoDefault(true);
    close->setDefault(true);
  }
}

void csWProgressLogger::monitorProgress(int value)
{
  if( value >= ui->progressBar->maximum() ) {
    finish();
  }
}

////// private ///////////////////////////////////////////////////////////////

QProgressBar *csWProgressLogger::progressBar()
{
  return ui->progressBar;
}
