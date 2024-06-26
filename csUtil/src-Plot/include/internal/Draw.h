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

#include <Plot/Plot.h>
#include <Plot/PlotRange.h>

class QPainter;
class QPen;
class QRectF;
class QTransform;

namespace plot {

  class IAxisElement;
  class PlotTheme;
  class Series;

  namespace Draw {

    void frame(QPainter *painter,
               const QRectF& rect, const QPen& pen);

    void gridX(QPainter *painter,
               const IAxisElement *xAxis, const QTransform& mapping,
               const QRectF& rect, const QPen& pen);

    void gridY(QPainter *painter,
               const IAxisElement *yAxis, const QTransform& mapping,
               const QRectF& rect, const QPen& pen);

    void series(QPainter *painter,
                const QRectF& screen, const Series& theSeries,
                const PlotRange& viewX, const PlotRange& viewY,
                const PlotTheme *theme, const DrawFlags flags = NoDrawFlags);

  } // namespace Draw

} // namespace plot
