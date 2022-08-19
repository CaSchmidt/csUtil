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

#include "cs/Logging/WProgressLogger.h"
#include "ui_WProgressLogger.h"

#include "cs/Logging/IProgress.h"
#include "cs/Qt/DialogButtonBox.h"

namespace cs {

  ////// Private /////////////////////////////////////////////////////////////

  namespace impl_prog {

    class IProgressImpl : public IProgress {
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

      void setProgressMaximum(const int max) const
      {
        _bar->setMaximum(max);
      }

      void setProgressMinimum(const int min) const
      {
        _bar->setMinimum(min);
      }

      void setProgressRange(const int min, const int max) const
      {
        _bar->setRange(min, max);
      }

      void setProgressValue(const int val) const
      {
        _bar->setValue(val);
      }

    private:
      IProgressImpl() noexcept = delete;

      QProgressBar *_bar;
    };

  } // namespace impl_prog

  ////// public //////////////////////////////////////////////////////////////

  WProgressLogger::WProgressLogger(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
    , ui{new Ui::WProgressLogger}
  {
    ui->setupUi(this);

    _progress = std::make_unique<impl_prog::IProgressImpl>(ui->progressBar);

    // User Interface ////////////////////////////////////////////////////////

    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    removeAllButtons(ui->buttonBox);

    QPushButton *cancel = addButton(ui->buttonBox, QDialogButtonBox::Cancel);
    if( cancel != nullptr ) {
      cancel->disconnect();

      connect(cancel, &QPushButton::clicked, this, &WProgressLogger::canceled);

      connect(cancel, &QPushButton::clicked, this, &WProgressLogger::rejected);
      connect(cancel, &QPushButton::clicked, this, &WProgressLogger::reject);
    }

    // Signals & Slots ///////////////////////////////////////////////////////

    connect(ui->progressBar, &QProgressBar::valueChanged,
            this, &WProgressLogger::valueChanged);
    connect(this, &WProgressLogger::valueChanged,
            this, &WProgressLogger::monitorProgress);
  }

  WProgressLogger::~WProgressLogger()
  {
    delete ui;
  }

  const ILogger *WProgressLogger::logger() const
  {
    return ui->logBrowser;
  }

  const IProgress *WProgressLogger::progress() const
  {
    return _progress.get();
  }

  ////// public slots ////////////////////////////////////////////////////////

  void WProgressLogger::stepValue(int dir)
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

  ////// private slots ///////////////////////////////////////////////////////

  void WProgressLogger::finish()
  {
    removeAllButtons(ui->buttonBox);

    QPushButton *close = addButton(ui->buttonBox, QDialogButtonBox::Close, true, true);
    if( close != nullptr ) {
      close->disconnect();

      connect(close, &QPushButton::clicked, this, &WProgressLogger::accepted);
      connect(close, &QPushButton::clicked, this, &WProgressLogger::accept);
    }
  }

  void WProgressLogger::monitorProgress(int value)
  {
    if( value >= ui->progressBar->maximum() ) {
      finish();
    }
  }

  ////// private /////////////////////////////////////////////////////////////

  QProgressBar *WProgressLogger::progressBar()
  {
    return ui->progressBar;
  }

} // namespace cs
