/****************************************************************************
** Copyright (c) 2017, Carsten Schmidt. All rights reserved.
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

#pragma once

#include <QtWidgets/QWidget>

#include <Plot/Plot.h>
#include <Plot/PlotSeriesHandle.h>
#include <Plot/PlotTheme.h>

class QMenu;

namespace plot {

  class IPlotImplementation;

  class CS_UTIL_EXPORT PlotWidget : public QWidget {
    Q_OBJECT
  public:
    PlotWidget(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~PlotWidget();

    PlotSeriesHandle handle(const QString& name) const;
    PlotSeriesHandle insert(IPlotSeriesData *data, const QColor& color = QColor());

    DrawFlags drawFlags() const;
    void setDrawFlags(const DrawFlags flags);
    void setDrawFlag(const DrawFlag flag, const bool on = true);

    PlotTheme theme() const;
    void setTheme(const PlotTheme& t);

    void setTitleX(const QString& title);

  public slots:
    void nextActiveSeries();
    void setActiveSeries(const QString& seriesName);
    void exportToClipboard();
    void panning();
    void horizontalZoom();
    void verticalZoom();
    void rectangularZoom();
    void reset();

  protected:
    void changeEvent(QEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void enterEvent(QEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void leaveEvent(QEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

  private slots:
    void setDrawMarks(bool on);
    void setDrawSteps(bool on);

  private:
    enum PanZoom {
      RectangularZoom = 0,
      HorizontalZoom,
      VerticalZoom,
      Panning
    };

    QAction *createAction(const QString& text, const QKeySequence& shortcut,
                          const bool add_menu = true);
    void initializeContextMenu();
    void initializeCursor();
    void updateMenuFlags();

    IPlotImplementation *_impl;
    QMenu *_contextMenu;
    QAction *_marksAction;
    QAction *_stepsAction;
    PanZoom _panZoom;
    QPoint _dragStart;
    QRect _zoomRect;
  };

} // namespace plot
