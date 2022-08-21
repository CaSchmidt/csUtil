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

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>

#include "cs/Qt/WListEditor.h"

#include "cs/Qt/Widget.h"

namespace cs {

  namespace impl_lstedt {

    void setupButton(QPushButton *button, const QString& iconPath)
    {
      button->setIcon(QIcon(iconPath));
      button->setText(QString());
    }

  } // namespace impl_lstedt

  ////// public //////////////////////////////////////////////////////////////

  WListEditor::WListEditor(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
  {
    setupUi();

    // Signals & Slots ///////////////////////////////////////////////////////

    connect(_addButton,    &QPushButton::clicked, this, &WListEditor::onAdd);
    connect(_downButton,   &QPushButton::clicked, this, &WListEditor::onDown);
    connect(_removeButton, &QPushButton::clicked, this, &WListEditor::onRemove);
    connect(_upButton,     &QPushButton::clicked, this, &WListEditor::onUp);
  }

  WListEditor::~WListEditor()
  {
  }

  ////// protected ///////////////////////////////////////////////////////////

  const QPushButton *WListEditor::button(const Button id) const
  {
    return const_cast<WListEditor*>(this)->button(id);
  }

  QPushButton *WListEditor::button(const Button id)
  {
    if(        id == Add ) {
      return _addButton;
    } else if( id == Down ) {
      return _downButton;
    } else if( id == Remove ) {
      return _removeButton;
    } else if( id == Up ) {
      return _upButton;
    }
    return nullptr;
  }

  void WListEditor::setShowContextMenu(const bool on)
  {
    if( on ) {
      _listView->setContextMenuPolicy(Qt::CustomContextMenu);
      connect(_listView, &QListView::customContextMenuRequested,
              this, &WListEditor::showContextMenu);
    } else {
      _listView->setContextMenuPolicy(Qt::NoContextMenu);
      disconnect(_listView, &QListView::customContextMenuRequested,
                 this, &WListEditor::showContextMenu);
    }
  }

  const QListView *WListEditor::view() const
  {
    return _listView;
  }

  QListView *WListEditor::view()
  {
    return _listView;
  }

  ////// private slots ///////////////////////////////////////////////////////

  void WListEditor::onAdd()
  {
  }

  void WListEditor::onContextMenu(const QPoint& globalPos)
  {
    Q_UNUSED(globalPos);
  }

  void WListEditor::onDown()
  {
  }

  void WListEditor::onRemove()
  {
  }

  void WListEditor::onUp()
  {
  }

  void WListEditor::showContextMenu(const QPoint& p)
  {
    onContextMenu(cs::mapToGlobal(_listView, p));
  }

  ////// private /////////////////////////////////////////////////////////////

  void WListEditor::setupUi()
  {
    constexpr int MY_MARGIN  = 0;
    constexpr int MY_SPACING = 4;

    // (1) Grid Layout ///////////////////////////////////////////////////////

    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(MY_MARGIN, MY_MARGIN, MY_MARGIN, MY_MARGIN);
    layout->setSpacing(MY_SPACING);

    // (2) List View /////////////////////////////////////////////////////////

    _listView = new QListView(this);
    layout->addWidget(_listView, 0, 0, 6, 1);

    // (3) Add Button ////////////////////////////////////////////////////////

    _addButton = new QPushButton(tr("add"), this);
    impl_lstedt::setupButton(_addButton, QStringLiteral(":/icons/plus.svg"));
    layout->addWidget(_addButton, 0, 1);

    // (4) Spacer #1 /////////////////////////////////////////////////////////

    // layout->setRowMinimumHeight(1, MY_HEIGHT);

    // (5) Up Button /////////////////////////////////////////////////////////

    _upButton = new QPushButton(tr("up"), this);
    impl_lstedt::setupButton(_upButton, QStringLiteral(":/icons/up.svg"));
    layout->addWidget(_upButton, 2, 1);

    // (6) Down Button ///////////////////////////////////////////////////////

    _downButton = new QPushButton(tr("down"), this);
    impl_lstedt::setupButton(_downButton, QStringLiteral(":/icons/down.svg"));
    layout->addWidget(_downButton, 3, 1);

    // (7) Spacer #2 /////////////////////////////////////////////////////////

    // layout->setRowMinimumHeight(4, MY_HEIGHT);

    // (8) Remove Button /////////////////////////////////////////////////////

    _removeButton = new QPushButton(tr("remove"), this);
    impl_lstedt::setupButton(_removeButton, QStringLiteral(":/icons/minus.svg"));
    layout->addWidget(_removeButton, 5, 1);
  }

} // namespace cs
