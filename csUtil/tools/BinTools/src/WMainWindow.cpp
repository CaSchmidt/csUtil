/****************************************************************************
** Copyright (c) 2024, Carsten Schmidt. All rights reserved.
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

#include <cs/Qt/Widget.h>

#include "WMainWindow.h"
#include "ui_WMainWindow.h"

#include "Encoder/WEncoderPage.h"
#include "global.h"

////// public ////////////////////////////////////////////////////////////////

WMainWindow::WMainWindow(QWidget *parent, const Qt::WindowFlags flags)
  : QMainWindow(parent, flags)
  , ui(std::make_unique<Ui::WMainWindow>())
{
  ui->setupUi(this);

  // Global Logger ///////////////////////////////////////////////////////////

  global::logger = ui->logBrowser->logger();

  // Signals & Slots /////////////////////////////////////////////////////////

  connect(ui->quitAction, &QAction::triggered,
          this, &WMainWindow::close);

  connect(ui->newEncoderAction, &QAction::triggered,
          this, &WMainWindow::newEncoderTab);
  connect(ui->closeTabAction, &QAction::triggered,
          this, &WMainWindow::closeCurrentTab);
  connect(ui->closeAllTabsAction, &QAction::triggered,
          this, &WMainWindow::closeAllTabs);

  connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
          this, &WMainWindow::removeTab);
}

WMainWindow::~WMainWindow()
{
}

////// private slots /////////////////////////////////////////////////////////

void WMainWindow::closeAllTabs()
{
  const int numTabs = ui->tabWidget->count();
  for(int i = 0; i < numTabs; i++) {
    removeTab(0);
  }
}

void WMainWindow::closeCurrentTab()
{
  removeTab(ui->tabWidget->currentIndex());
}

void WMainWindow::newEncoderTab()
{
  ui->tabWidget->addTab(new WEncoderPage, QStringLiteral("Encoder"));
}

void WMainWindow::removeTab(const int index)
{
  cs::WidgetPtr widget(ui->tabWidget->widget(index));
  if( widget ) {
    ui->tabWidget->removeTab(index);
  }
}
