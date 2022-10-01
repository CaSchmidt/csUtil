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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QPushButton>

#include "cs/Logging/WProgressLogger.h"

#include "cs/Logging/IProgress.h"
#include "cs/Logging/WLogger.h"
#include "cs/Qt/DialogButtonBox.h"
#include "cs/Qt/FilterCancel.h"
#include "cs/Qt/Layout.h"

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

      void resetProgress() const
      {
        setProgressRange(0, 100);
        setProgressValue(0);
      }

      void setProgressRange(const int min, const int max) const
      {
        QMetaObject::invokeMethod(_bar, "setRange", Qt::AutoConnection,
                                  Q_ARG(int, min), Q_ARG(int, max));
      }

      void setProgressValue(const int val) const
      {
        QMetaObject::invokeMethod(_bar, "setValue", Qt::AutoConnection,
                                  Q_ARG(int, val));
      }

    private:
      IProgressImpl() noexcept = delete;

      QProgressBar *_bar;
    };

  } // namespace impl_prog

  ////// public //////////////////////////////////////////////////////////////

  WProgressLogger::WProgressLogger(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
  {
    setupUi();

    _progress = std::make_unique<impl_prog::IProgressImpl>(_progressBar);

    // User Interface ////////////////////////////////////////////////////////

    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    removeAllButtons(_buttonBox);
    startUi();

    // Event Filter //////////////////////////////////////////////////////////

    FilterCancel *filter = new FilterCancel(this);
    installEventFilter(filter);

    // Signals & Slots ///////////////////////////////////////////////////////

    connect(_progressBar, &QProgressBar::valueChanged,
            this, &WProgressLogger::valueChanged);
    connect(this, &WProgressLogger::valueChanged,
            this, &WProgressLogger::monitorProgress);
  }

  WProgressLogger::~WProgressLogger()
  {
  }

  const ILogger *WProgressLogger::logger() const
  {
    return _logger;
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
    const int min = _progressBar->minimum();
    const int val = _progressBar->value();
    const int max = _progressBar->maximum();
    _progressBar->setValue(qBound<int>(min, val + inc, max));
  }

  ////// private slots ///////////////////////////////////////////////////////

  void WProgressLogger::finishUi()
  {
    const QDialogButtonBox::StandardButton which = QDialogButtonBox::Close;

    if( isUnique(_buttonBox, which) ) {
      return;
    }

    removeAllButtons(_buttonBox);

    QPushButton *button = addButton(_buttonBox, which, true, true);
    if( button != nullptr ) {
      button->disconnect();

      connect(button, &QPushButton::clicked, this, &WProgressLogger::accepted);
      connect(button, &QPushButton::clicked, this, &WProgressLogger::accept);
    }
  }

  void WProgressLogger::monitorProgress(int value)
  {
    if( value >= _progressBar->maximum() ) {
      finishUi();
    } else {
      startUi();
    }
  }

  void WProgressLogger::startUi()
  {
    const QDialogButtonBox::StandardButton which = QDialogButtonBox::Cancel;

    if( isUnique(_buttonBox, which) ) {
      return;
    }

    removeAllButtons(_buttonBox);

    QPushButton *button = addButton(_buttonBox, which);
    if( button != nullptr ) {
      button->disconnect();

      connect(button, &QPushButton::clicked, this, &WProgressLogger::canceled);

      connect(button, &QPushButton::clicked, this, &WProgressLogger::rejected);
      connect(button, &QPushButton::clicked, this, &WProgressLogger::reject);
    }
  }

  ////// private /////////////////////////////////////////////////////////////

  void WProgressLogger::setupUi()
  {
    constexpr int MY_MARGIN  = 4;
    constexpr int MY_SPACING = 4;

    addLayout<QVBoxLayout>(this, MY_MARGIN, MY_SPACING);
    resize(640, 480);

    // (1) Progress //////////////////////////////////////////////////////////
    {
      QGroupBox *progressGroup = new QGroupBox(tr("Progress"), this);
      layout()->addWidget(progressGroup);

      addLayout<QVBoxLayout>(progressGroup, MY_MARGIN, MY_SPACING);

      _progressBar = new QProgressBar(progressGroup);
      progressGroup->layout()->addWidget(_progressBar);
    }

    // (2) Output ////////////////////////////////////////////////////////////
    {
      QGroupBox *outputGroup = new QGroupBox(tr("Output"), this);
      layout()->addWidget(outputGroup);

      addLayout<QVBoxLayout>(outputGroup, MY_MARGIN, MY_SPACING);

      _logger = new WLogger(outputGroup);
      outputGroup->layout()->addWidget(_logger);
    }

    // (3) Button Box ////////////////////////////////////////////////////////
    {
      _buttonBox = new QDialogButtonBox(this);
      layout()->addWidget(_buttonBox);
    }

    // Tab Order /////////////////////////////////////////////////////////////

    QWidget::setTabOrder(_progressBar, _logger);
    QWidget::setTabOrder(_logger, _buttonBox);
  }

} // namespace cs
