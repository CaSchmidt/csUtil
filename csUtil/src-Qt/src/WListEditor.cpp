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

#include "cs/Qt/WListEditor.h"
#include "ui_WListEditor.h"

#include "cs/Qt/Widget.h"

////// public ////////////////////////////////////////////////////////////////

csWListEditor::csWListEditor(QWidget *parent, Qt::WindowFlags f)
  : QWidget(parent, f)
  , ui{new Ui::csWListEditor}
{
  ui->setupUi(this);

  // Signals & Slots /////////////////////////////////////////////////////////

  connect(ui->addButton,    &QPushButton::clicked, this, &csWListEditor::onAdd);
  connect(ui->downButton,   &QPushButton::clicked, this, &csWListEditor::onDown);
  connect(ui->removeButton, &QPushButton::clicked, this, &csWListEditor::onRemove);
  connect(ui->upButton,     &QPushButton::clicked, this, &csWListEditor::onUp);
}

csWListEditor::~csWListEditor()
{
  delete ui;
}

////// protected /////////////////////////////////////////////////////////////

const QPushButton *csWListEditor::button(const Button id) const
{
  return const_cast<csWListEditor*>(this)->button(id);
}

QPushButton *csWListEditor::button(const Button id)
{
  if(        id == Add ) {
    return ui->addButton;
  } else if( id == Down ) {
    return ui->downButton;
  } else if( id == Remove ) {
    return ui->removeButton;
  } else if( id == Up ) {
    return ui->upButton;
  }
  return nullptr;
}

void csWListEditor::setShowContextMenu(const bool on)
{
  if( on ) {
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView, &QListView::customContextMenuRequested,
            this, &csWListEditor::showContextMenu);
  } else {
    ui->listView->setContextMenuPolicy(Qt::NoContextMenu);
    disconnect(ui->listView, &QListView::customContextMenuRequested,
               this, &csWListEditor::showContextMenu);
  }
}

const QListView *csWListEditor::view() const
{
  return ui->listView;
}

QListView *csWListEditor::view()
{
  return ui->listView;
}

////// private slots /////////////////////////////////////////////////////////

void csWListEditor::onAdd()
{
}

void csWListEditor::onContextMenu(const QPoint& globalPos)
{
  Q_UNUSED(globalPos);
}

void csWListEditor::onDown()
{
}

void csWListEditor::onRemove()
{
}

void csWListEditor::onUp()
{
}

void csWListEditor::showContextMenu(const QPoint& p)
{
  onContextMenu(cs::mapToGlobal(ui->listView, p));
}
