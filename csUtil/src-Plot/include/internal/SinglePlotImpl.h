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

#include "internal/IPlotImplementation.h"

namespace plot {

  class IAxisElement;
  class ITitleElement;
  class ScopeRow;

  class SinglePlotImpl : public IPlotImplementation {
  public:
    SinglePlotImpl(const DrawFlags drawFlags, const PlotTheme& theme, QWidget *widget);
    ~SinglePlotImpl();

    void replot();
    void reset();
    const QWidget *widget() const;

    // IPlotElement

    QRectF boundingRect() const;
    void resize(const QPointF& topLeft, const QSizeF& hint);
    void paint(QPainter *painter) const;

    // ISeriesControl

    PlotSeriesHandle handle(const QString& name) const;
    PlotSeriesHandle insert(IPlotSeriesData *data, const QColor& color);
    bool remove(const QString& name);
    const Series& series(const QString& name) const;
    Series& series(const QString& name);
    bool setActiveSeries(const QString& name);
    bool setNextActiveSeries();

    // IViewportControl

    void pan(const QPointF& delta);
    void rectangularZoom(const QRectF& zoomRect);
    void horizontalZoom(const QRectF& zoomRect);
    void verticalZoom(const QRectF& zoomRect);

    // IXAxisControl

    QTransform mapViewToScreenX() const;
    PlotRange rangeX() const;
    void setTitleX(const QString& title);
    PlotRange totalRangeX() const;
    const IAxisElement *xAxis() const;

  private:
    QWidget *_widget;
    PlotRange _rangeX;
    QRectF _rect;
    ScopeRow *_row;
    IAxisElement *_xAxis;
    ITitleElement *_xTitle;
  };

} // namespace plot
