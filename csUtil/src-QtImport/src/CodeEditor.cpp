/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui/QPainter>
#include <QtGui/QTextBlock>

#include "QtExamples/CodeEditor.h"

#include "cs/Core/Convert.h"
#include "cs/Core/QStringUtil.h"
#include "cs/Math/Numeric.h"

namespace QtExamples {

  ////// LineNumberArea - Implementation /////////////////////////////////////

  namespace impl_editor {

    class LineNumberArea : public QWidget {
    public:
      LineNumberArea(CodeEditor *editor)
        : QWidget(editor)
        , _editor{editor}
      {
      }

      ~LineNumberArea()
      {
      }

      QSize sizeHint() const override
      {
        return QSize(_editor->lineNumberAreaWidth(), 0);
      }

    protected:
      void paintEvent(QPaintEvent *event) override
      {
        _editor->lineNumberAreaPaintEvent(event);
      }

    private:
      CodeEditor *_editor{nullptr};
    };

  } // namespace impl_editor

  ////// public //////////////////////////////////////////////////////////////

  CodeEditor::CodeEditor(QWidget *parent)
    : QPlainTextEdit(parent)
  {
    _lineNumberArea = new impl_editor::LineNumberArea(this);

    connect(this, &CodeEditor::blockCountChanged,
            this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::cursorPositionChanged,
            this, &CodeEditor::highlightCurrentLine);
    connect(this, &CodeEditor::updateRequest,
            this, &CodeEditor::updateLineNumberArea);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
  }

  CodeEditor::~CodeEditor()
  {
  }

  ////// protected ///////////////////////////////////////////////////////////

  void CodeEditor::resizeEvent(QResizeEvent *event)
  {
    QPlainTextEdit::resizeEvent(event);

    const QRect cr = contentsRect();
    _lineNumberArea->setGeometry(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height());
  }

  ////// private slots ///////////////////////////////////////////////////////

  void CodeEditor::highlightCurrentLine()
  {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if( !isReadOnly() ) {
      QTextEdit::ExtraSelection selection;

      const QColor color = QColor(Qt::yellow).lighter(160);

      selection.format.setBackground(color);
      selection.format.setProperty(QTextFormat::FullWidthSelection, true);
      selection.cursor = textCursor();
      selection.cursor.clearSelection();

      extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
  }

  void CodeEditor::updateLineNumberArea(const QRect& rect, int dy)
  {
    if( dy != 0 ) {
      _lineNumberArea->scroll(0, dy);
    } else {
      _lineNumberArea->update(0, rect.y(), _lineNumberArea->width(), rect.height());
    }

    if( rect.contains(viewport()->rect()) ) {
      updateLineNumberAreaWidth(0);
    }
  }

  void CodeEditor::updateLineNumberAreaWidth(int)
  {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
  }

  ////// private /////////////////////////////////////////////////////////////

  void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
  {
    QPainter painter(_lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();

    for(; block.isValid() && top <= event->rect().bottom(); block = block.next()) {
      const int bottom = top + (int)blockBoundingRect(block).height();

      if( block.isVisible() && bottom >= event->rect().top() ) {
        const QString number = QString::number(block.blockNumber() + FIRST_LINE);
        painter.setPen(Qt::black);
        painter.drawText(0, top,
                         _lineNumberArea->width(), fontMetrics().height(),
                         Qt::AlignRight, number);
      }

      top = bottom;
    } // For Each Block (Line)
  }

  int CodeEditor::lineNumberAreaWidth() const
  {
    const int digits = cs::toSigned<int>(cs::countDigits(blockCount()));

    const int space = 3 + fontMetrics().horizontalAdvance(cs::L1C('9'))*digits;

    return space;
  }

} // namespace QtExamples
