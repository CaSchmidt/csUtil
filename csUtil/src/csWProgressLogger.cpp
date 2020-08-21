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

////// Private ///////////////////////////////////////////////////////////////

namespace priv {

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

  connect(ui->progressBar, &QProgressBar::valueChanged, this, &csWProgressLogger::valueChanged);
}

csWProgressLogger::~csWProgressLogger()
{
  delete ui;
}

const csILogger *csWProgressLogger::logger() const
{
  return ui->logBrowser;
}

int csWProgressLogger::maximum() const
{
  return ui->progressBar->maximum();
}

int csWProgressLogger::minimum() const
{
  return ui->progressBar->minimum();
}

int csWProgressLogger::value() const
{
  return ui->progressBar->value();
}

////// public slots //////////////////////////////////////////////////////////

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

void csWProgressLogger::setMaximum(int maximum)
{
  ui->progressBar->setMaximum(maximum);
}

void csWProgressLogger::setMinimum(int minimum)
{
  ui->progressBar->setMinimum(minimum);
}

void csWProgressLogger::setRange(int minimum, int maximum)
{
  ui->progressBar->setRange(minimum, maximum);
}

void csWProgressLogger::setValue(int value)
{
  ui->progressBar->setValue(value);
}

void csWProgressLogger::stepValue(int dir)
{
  if( dir == 0 ) {
    return;
  }
  const int inc = dir >= 0
      ? 1
      : -1;
  setValue(qBound<int>(minimum(), value() + inc, maximum()));
}
