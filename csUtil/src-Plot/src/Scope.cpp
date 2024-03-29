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

#include "Plot/PlotTheme.h"

#include "internal/Scope.h"

#include "internal/Draw.h"
#include "internal/IPlotImplementation.h"
#include "internal/Mapping.h"
#include "internal/ScopeRow.h"

namespace plot {

  ////// public //////////////////////////////////////////////////////////////

  Scope::Scope(ScopeRow *row)
    : _rect()
    , _row(row)
  {
  }

  Scope::~Scope()
  {
  }

  QRectF Scope::boundingRect() const
  {
    return _rect;
  }

  void Scope::resize(const QPointF& topLeft, const QSizeF& hint)
  {
    _rect = QRectF(topLeft, hint);
  }

  void Scope::paint(QPainter *painter) const
  {
    const IPlotImplementation *plot = _row->plot();

    painter->save();

    painter->setBrush(Qt::NoBrush);

    // (1) Grid //////////////////////////////////////////////////////////////

    const QTransform mapping = _row->mapScaleToScreen();

    Draw::gridX(painter,
                plot->xAxis(), mapping, _rect, plot->theme()->pen(PlotTheme::Grid));
    Draw::gridY(painter,
                _row->yAxis(), mapping, _rect, plot->theme()->pen(PlotTheme::Grid));

    // (2) Frame /////////////////////////////////////////////////////////////

    Draw::frame(painter,
                _rect, plot->theme()->pen(PlotTheme::Frame));

    // (3) Series (Excluding Active Series) //////////////////////////////////

    const Series& activeSeries = _row->activeSeries();
    const PlotRange  rangeX = plot->rangeX();

    painter->resetTransform();
    painter->setClipRect(_rect);

    const QStringList seriesNames = _row->store().names();
    for(const QString& seriesName : seriesNames) {
      const Series& series = _row->store().series(seriesName);
      if( series.isEmpty()  ||  series == activeSeries ) {
        continue;
      }
      const PlotRange rangeY =
          _row->store().rangeY(series.name()).subset(_row->viewY(), 100);
      Draw::series(painter,
                   _rect, series, rangeX, rangeY,
                   plot->theme(), plot->drawFlags());
    }

    // (4) Active Series /////////////////////////////////////////////////////

    if( !activeSeries.isEmpty() ) {
      painter->resetTransform();
      painter->setClipRect(_rect.adjusted(-1, -1, 1, 1));
      const PlotRange rangeY =
          _row->store().rangeY(activeSeries.name()).subset(_row->viewY(), 100);
      Draw::series(painter,
                   _rect, activeSeries, rangeX, rangeY,
                   plot->theme(), plot->drawFlags() | IsActive);
    }

    painter->restore();
  }

} // namespace plot
