/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#pragma once

#include <QtGui/QTextLayout>
#include <QtWidgets/QItemDelegate>

#include <cs/Core/csutil_config.h>

namespace QtCreator {

  enum class HighlightingItemRole {
    LineNumber = Qt::UserRole, // -> int
    StartColumn,               // -> QVector<int>
    Length,                    // -> QVector<int>
    Foreground,                // -> QColor
    Background,                // -> QColor
    User
  };

  class CS_UTIL_EXPORT HighlightingItemDelegate : public QItemDelegate {
    Q_OBJECT
    Q_PROPERTY(int tabWidth READ tabWidth WRITE setTabWidth NOTIFY tabWidthChanged)
  public:
    HighlightingItemDelegate(int tabWidth, QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
    void setTabWidth(int width);
    int tabWidth() const;

  private:
    int drawLineNumber(QPainter *painter, const QStyleOptionViewItem& option, const QRect& rect,
                       const QModelIndex& index) const;
    void drawText(QPainter *painter, const QStyleOptionViewItem& option,
                  const QRect& rect, const QModelIndex& index) const;
    using QItemDelegate::drawDisplay;
    void drawDisplay(QPainter *painter, const QStyleOptionViewItem& option, const QRect& rect,
                     const QString& text, const QVector<QTextLayout::FormatRange>& format) const;

    QString m_tabString;

  signals:
    void tabWidthChanged(int width);
  };

} // namespace QtCreator
