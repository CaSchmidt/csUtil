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

#include <QtGui/QTransform>

#include "internal/PanAndZoom.h"

#include "internal/Mapping.h"

namespace plot {

  namespace Pan {

    PlotRange horizontal(const QPointF& delta, const QSizeF& screen,
                         const PlotRange& viewX,
                         const PlotRange& viewY,
                         const PlotRange& boundsX)
    {
      if( !viewX.isValid()  ||  !viewY.isValid()  ||  !boundsX.isValid()  ||  screen.isEmpty() ) {
        return PlotRange();
      }

      const QTransform  xform = Mapping::screenToView(screen, viewX, viewY, true);
      const QPointF deltaView = xform.map(delta);

      // Horizontal

      qreal leftX = viewX.begin - deltaView.x();
      if(        leftX < boundsX.begin ) {
        leftX += boundsX.begin - leftX;
      } else if( leftX + viewX.range() > boundsX.end ) {
        leftX -= leftX + viewX.range() - boundsX.end;
      }

      return PlotRange(leftX, leftX + viewX.range());
    }

    PlotRange vertical(const QPointF& delta, const QSizeF& screen,
                       const PlotRange& viewX,
                       const PlotRange& viewY,
                       const PlotRange& boundsY)
    {
      if( !viewX.isValid()  ||  !viewY.isValid()  ||  !boundsY.isValid()  ||  screen.isEmpty() ) {
        return PlotRange();
      }

      const QTransform  xform = Mapping::screenToView(screen, viewX, viewY, true);
      const QPointF deltaView = xform.map(delta);

      // Vertical

      qreal bottomY = viewY.begin + deltaView.y();
      if(        bottomY < boundsY.begin ) {
        bottomY += boundsY.begin - bottomY;
      } else if( bottomY + viewY.range() > boundsY.end ) {
        bottomY -= bottomY + viewY.range() - boundsY.end;
      }

      return PlotRange(bottomY, bottomY + viewY.range());
    }

  } // namespace Pan

  namespace ZoomIn {

    QRectF rectangular(const QRectF& zoomRect, const QRectF& screen,
                       const PlotRange& viewX,
                       const PlotRange& viewY)
    {
      if( !viewX.isValid()  ||  !viewY.isValid()  ||  screen.isEmpty() ) {
        return QRectF();
      }

      const QRectF inter = screen & zoomRect;
      if( inter.isEmpty() ) {
        return QRectF();
      }

      const QTransform xform = Mapping::screenToView(screen.size(), viewX, viewY);
      const QRectF  zoomView =
          xform.map(inter.translated(-screen.topLeft())).boundingRect();

      const QRectF view(QPointF(viewX.begin, viewY.begin),
                        QPointF(viewX.end, viewY.end));

      return view & zoomView;
    }

  } // namespace ZoomIn

} // namespace plot
