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

#include <QtGui/QGuiApplication>
#include <QtGui/QClipboard>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QFocusEvent>
#include <QtGui/QImage>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QResizeEvent>
#include <QtWidgets/QMenu>

#include "Plot/PlotWidget.h"

#include "internal/SinglePlotImpl.h"

namespace plot {

  ////// public //////////////////////////////////////////////////////////////

  PlotWidget::PlotWidget(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , _impl()
    , _contextMenu()
    , _panZoom(RectangularZoom)
    , _dragStart()
    , _zoomRect()
  {
    _impl = new SinglePlotImpl(NoDrawFlags, PlotTheme::themeTextbook(), this);

    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    initializeContextMenu();
    initializeCursor();

    QAction *action = createAction(tr("Next series"), Qt::Key_Tab, false);
    connect(action, &QAction::triggered, this, &PlotWidget::nextActiveSeries);
  }

  PlotWidget::~PlotWidget()
  {
    delete _impl;
  }

  PlotSeriesHandle PlotWidget::handle(const QString& name) const
  {
    return _impl->handle(name);
  }

  PlotSeriesHandle PlotWidget::insert(IPlotSeriesData *data,
                                      const QColor& color)
  {
    return _impl->insert(data, color);
  }

  DrawFlags PlotWidget::drawFlags() const
  {
    return _impl->drawFlags();
  }

  void PlotWidget::setDrawFlags(const DrawFlags flags)
  {
    _impl->setDrawFlags(flags);
    updateMenuFlags();
  }

  void PlotWidget::setDrawFlag(const DrawFlag flag, const bool on)
  {
    _impl->setDrawFlag(flag, on);
    updateMenuFlags();
  }

  void PlotWidget::setTitleX(const QString& title)
  {
    _impl->setTitleX(title);
  }

  ////// public slots ////////////////////////////////////////////////////////

  void PlotWidget::nextActiveSeries()
  {
    _impl->setNextActiveSeries();
  }

  void PlotWidget::setActiveSeries(const QString& seriesName)
  {
    _impl->setActiveSeries(seriesName);
  }

  void PlotWidget::exportToClipboard()
  {
    QImage image(size(), QImage::Format_ARGB32_Premultiplied);
    render(&image);
    QGuiApplication::clipboard()->setImage(image);
  }

  void PlotWidget::panning()
  {
    _panZoom = Panning;
    initializeCursor();
  }

  void PlotWidget::horizontalZoom()
  {
    _panZoom = HorizontalZoom;
    initializeCursor();
  }

  void PlotWidget::verticalZoom()
  {
    _panZoom = VerticalZoom;
    initializeCursor();
  }

  void PlotWidget::rectangularZoom()
  {
    _panZoom = RectangularZoom;
    initializeCursor();
  }

  void PlotWidget::reset()
  {
    _impl->reset();
  }

  ////// protected /////////////////////////////////////////////////////////////

  void PlotWidget::changeEvent(QEvent *event)
  {
    QWidget::changeEvent(event);
    if( event->type() == QEvent::FontChange ) {
      _impl->replot();
    }
  }

  void PlotWidget::contextMenuEvent(QContextMenuEvent *event)
  {
    _contextMenu->exec(event->globalPos());
    event->accept();
  }

  void PlotWidget::enterEvent(QEvent *event)
  {
    setFocus();
    QWidget::enterEvent(event);
  }

  void PlotWidget::focusOutEvent(QFocusEvent *event)
  {
    initializeCursor();
    QWidget::focusOutEvent(event);
  }

  void PlotWidget::leaveEvent(QEvent *event)
  {
    initializeCursor();
    QWidget::leaveEvent(event);
  }

  void PlotWidget::mouseMoveEvent(QMouseEvent *event)
  {
    if( event->buttons().testFlag(Qt::LeftButton) ) {
      if( _panZoom == Panning ) {
        const QPointF delta = QPointF(event->pos()) - _dragStart;
        _dragStart = event->pos();
        _impl->pan(delta);
      } else {
        _zoomRect = QRect(_dragStart, event->pos()).normalized();
        update();
      }
    }
    event->accept();
  }

  void PlotWidget::mousePressEvent(QMouseEvent *event)
  {
    if( event->buttons() == Qt::LeftButton ) {
      _dragStart = event->pos();
      if( _panZoom == Panning ) {
        setCursor(Qt::ClosedHandCursor);
      }
    }
    event->accept();
  }

  void PlotWidget::mouseReleaseEvent(QMouseEvent *event)
  {
    if(        _panZoom == RectangularZoom ) {
      _impl->rectangularZoom(_zoomRect);
    } else if( _panZoom == HorizontalZoom ) {
      _zoomRect.setTop(0);
      _zoomRect.setHeight(height());
      _impl->horizontalZoom(_zoomRect);
    } else if( _panZoom == VerticalZoom ) {
      _zoomRect.setLeft(0);
      _zoomRect.setWidth(width());
      _impl->verticalZoom(_zoomRect);
    } else if( _panZoom == Panning ) {
      initializeCursor();
    }
    _zoomRect = QRect();
    event->accept();
  }

  void PlotWidget::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    _impl->paint(&painter);

    painter.save();

    if( !_zoomRect.isNull() ) {
      const QRectF r(_zoomRect);

      painter.setPen(_impl->theme().rubberPen());
      if(        _panZoom == RectangularZoom ) {
        painter.drawRect(r.adjusted(0.5, 0.5, -0.5, -0.5));
      } else if( _panZoom == HorizontalZoom ) {
        const QPointF beginL(r.left() + 0.5, 0);
        const QPointF   endL(r.left() + 0.5, height());
        painter.drawLine(beginL, endL);

        const QPointF beginR(r.right() - 0.5, 0);
        const QPointF   endR(r.right() - 0.5, height());
        painter.drawLine(beginR, endR);
      } else if( _panZoom == VerticalZoom ) {
        const QPointF beginT(0,       r.top() + 0.5);
        const QPointF   endT(width(), r.top() + 0.5);
        painter.drawLine(beginT, endT);

        const QPointF beginB(0,       r.bottom() - 0.5);
        const QPointF   endB(width(), r.bottom() - 0.5);
        painter.drawLine(beginB, endB);
      }
    }

    painter.restore();

    event->accept();
  }

  void PlotWidget::resizeEvent(QResizeEvent *event)
  {
    _impl->resize(QPointF(0, 0), event->size());

    event->accept();
  }

  ////// private slots ///////////////////////////////////////////////////////

  void PlotWidget::setDrawMarks(bool on)
  {
    setDrawFlag(Marks, on);
  }

  void PlotWidget::setDrawSteps(bool on)
  {
    setDrawFlag(Steps, on);
  }

  ////// private /////////////////////////////////////////////////////////////

  QAction *PlotWidget::createAction(const QString& text,
                                    const QKeySequence& shortcut,
                                    const bool add_menu)
  {
    QAction *action = new QAction(text, this);
    addAction(action);
    if( add_menu ) {
      _contextMenu->addAction(action);
    }
    action->setShortcut(shortcut);
    action->setShortcutContext(Qt::WidgetShortcut);
    return action;
  }

  void PlotWidget::initializeContextMenu()
  {
    _contextMenu = new QMenu(this);

    QAction *clipboardAction = createAction(tr("Clipboard"), QKeySequence::Copy);
    connect(clipboardAction, &QAction::triggered, this, &PlotWidget::exportToClipboard);

    _contextMenu->addSeparator(); //////////////////////////////////////////////

    QAction *panAction = createAction(tr("Pan"), Qt::Key_M);
    connect(panAction, &QAction::triggered, this, &PlotWidget::panning);

    _contextMenu->addSeparator(); //////////////////////////////////////////////

    QAction *hzoomAction = createAction(tr("Horizontal"), Qt::Key_H);
    connect(hzoomAction, &QAction::triggered, this, &PlotWidget::horizontalZoom);

    QAction *vzoomAction = createAction(tr("Vertical"), Qt::Key_V);
    connect(vzoomAction, &QAction::triggered, this, &PlotWidget::verticalZoom);

    QAction *rzoomAction = createAction(tr("Rectangular"), Qt::Key_R);
    connect(rzoomAction, &QAction::triggered, this, &PlotWidget::rectangularZoom);

    _contextMenu->addSeparator(); //////////////////////////////////////////////

    QMenu *flagsMenu = _contextMenu->addMenu(tr("Flags"));

    _marksAction = flagsMenu->addAction(tr("Marks"));
    _marksAction->setCheckable(true);
    connect(_marksAction, &QAction::triggered, this, &PlotWidget::setDrawMarks);

    _stepsAction = flagsMenu->addAction(tr("Steps"));
    _stepsAction->setCheckable(true);
    connect(_stepsAction, &QAction::triggered, this, &PlotWidget::setDrawSteps);

    updateMenuFlags();

    _contextMenu->addSeparator(); //////////////////////////////////////////////

    QAction *resetAction = createAction(tr("Reset"), Qt::Key_Escape);
    connect(resetAction, &QAction::triggered, this, &PlotWidget::reset);

    setContextMenuPolicy(Qt::DefaultContextMenu);
  }

  void PlotWidget::initializeCursor()
  {
    if( _panZoom == Panning ) {
      setCursor(Qt::OpenHandCursor);
    } else {
      setCursor(Qt::CrossCursor);
    }
  }

  void PlotWidget::updateMenuFlags()
  {
    const DrawFlags f = drawFlags();

    _marksAction->setChecked(f.testFlag(Marks));
    _stepsAction->setChecked(f.testFlag(Steps));
  }

} // namespace plot
