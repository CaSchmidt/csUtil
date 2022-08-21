/****************************************************************************
** Copyright (c) 2013-2018, Carsten Schmidt. All rights reserved.
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

#include <QtCore/QPoint>
#include <QtCore/QPropertyAnimation>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtGui/QPixmap>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QGraphicsOpacityEffect>
#include <QtWidgets/QWidget>

#include "cs/Qt/ImageTip.h"

#include "cs/Qt/Widget.h"

namespace cs {

  ////// Private /////////////////////////////////////////////////////////////

  class TipWidget : public QWidget {
    Q_OBJECT
  public:
    TipWidget(const QImage& image,
              const ImageTip::Flags flags = ImageTip::NoFlags,
              QWidget *parent = nullptr, Qt::WindowFlags winFlags = 0);
    ~TipWidget();

    static TipWidget *instance;

    bool eventFilter(QObject *watched, QEvent *event);
    void hideTip();
    void placeTip(const QPoint& globalPos, QWidget *widget);

  protected:
    void paintEvent(QPaintEvent *event);

  private:
    QPixmap pixmap;
  };

  TipWidget *TipWidget::instance = nullptr;

  TipWidget::TipWidget(const QImage& image,
                       const ImageTip::Flags flags,
                       QWidget *parent, Qt::WindowFlags)
    : QWidget(parent,
              Qt::ToolTip | Qt::BypassGraphicsProxyWidget | Qt::FramelessWindowHint)
    , pixmap()
  {
    delete instance;
    instance = this;

    setAttribute(Qt::WA_TranslucentBackground, true);
    setObjectName(QLatin1String("csTipWidget"));

    pixmap = QPixmap::fromImage(image);
    if( flags.testFlag(ImageTip::DrawBorder) ) {
      QPainter painter(&pixmap);
      painter.setBackgroundMode(Qt::TransparentMode);
      painter.setBackground(QBrush(Qt::NoBrush));
      painter.setBrush(QBrush(Qt::NoBrush));
      painter.setPen(QPen(QBrush(Qt::black, Qt::SolidPattern),
                          0, Qt::SolidLine));
      painter.drawRect(0, 0, image.width()-1, image.height()-1);
    }

    resize(pixmap.width(), pixmap.height());

    setMouseTracking(true);

    QApplication::instance()->installEventFilter(this);
  }

  TipWidget::~TipWidget()
  {
    instance = nullptr;
  }

  bool TipWidget::eventFilter(QObject *, QEvent *event)
  {
    switch( event->type() ) {
    // NOTE: If the Image ToolTip is Placed with ZERO Offset to the
    //       Mouse's Cursor, the Image Will be immediately Hidden
    //       Due to FocusIn/Out, Leave, WindowActivate/Deactivate
    //       Events Passed to This Widget.
    case QEvent::FocusIn:
    case QEvent::FocusOut:
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::Leave:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
    case QEvent::Wheel:
    case QEvent::WindowActivate:
    case QEvent::WindowDeactivate:
      hideTip();
      break;

    default:
      break;
    }

    return false;
  }

  void TipWidget::hideTip()
  {
    close();
    deleteLater();
  }

  void TipWidget::placeTip(const QPoint& globalPos, QWidget *widget)
  {
    const QPoint offset(8, 8);

    QPoint placePos(offset+globalPos);

    const QRect screenRect = screenGeometry(globalPos, widget);

    if( placePos.x() + width() > screenRect.right()  &&
        width() < screenRect.width() ) {
      placePos.rx() = globalPos.x() - offset.x() - width();
    }

    if( placePos.y() + height() > screenRect.bottom()  &&
        height() < screenRect.height() ) {
      placePos.ry() = globalPos.y() - offset.y() - height();
    }

    move(placePos);
  }

  void TipWidget::paintEvent(QPaintEvent *event)
  {
    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawPixmap(0, 0, pixmap);

    event->accept();
  }

  ////// Public ////////////////////////////////////////////////////////////////

  void ImageTip::showImage(const QPoint& globalPos, const QImage& image,
                           QWidget *widget,
                           const ImageTip::Flags flags)
  {
#ifndef Q_WS_WIN
    new TipWidget(image, flags, widget);
#else
    new TipWidget(image, flags,
                  QApplication::desktop()->screen(screenNumber(globalPos,
                                                               widget)));
#endif

    if( flags.testFlag(ForcePosition) ) {
      TipWidget::instance->move(globalPos);
    } else {
      TipWidget::instance->placeTip(globalPos, widget);
    }

    if( flags.testFlag(ImageTip::NoEffects) ) {
      TipWidget::instance->show();

    } else {
      QGraphicsOpacityEffect *opacity =
          new QGraphicsOpacityEffect(TipWidget::instance);
      opacity->setOpacity(0);
      TipWidget::instance->setGraphicsEffect(opacity);

      QPropertyAnimation *animation =
          new QPropertyAnimation(opacity, "opacity", TipWidget::instance);
      animation->setDuration(250);
      animation->setStartValue(qreal(0));
      animation->setEndValue(qreal(1));
      animation->setEasingCurve(QEasingCurve::InQuad);

      TipWidget::instance->show();
      animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
  }

} // namespace cs

#include "ImageTip.moc"
