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

#include <QtGui/QPainter>

#include "Plot/PlotTheme.h"

#include "internal/Draw.h"

#include "internal/IAxisElement.h"
#include "internal/Mapping.h"
#include "internal/Series.h"

namespace plot {

  namespace Draw {

    namespace impl_draw {

      struct LinesHelper {
        static constexpr int LINES = 32;

        LinesHelper() noexcept = default;

        inline void draw(QPainter *painter, const IPlotSeriesData *data,
                         const int L, const int numLines) const
        {
          data->values(_points, L, L + numLines);
          painter->drawPolyline(_points, numLines + 1);
        }

      private:
        static QPointF _points[LINES + 1];
      };

      QPointF LinesHelper::_points[LinesHelper::LINES + 1];

      void drawLines(QPainter *painter,
                     const IPlotSeriesData *data, const int L, const int R)
      {
        const int numData = R - L + 1;
        if( numData < 2 ) {
          return;
        }

        LinesHelper helper;

        int i = L;
        while( i + LinesHelper::LINES <= R ) {
          helper.draw(painter, data, i, LinesHelper::LINES);
          i += LinesHelper::LINES;
        }

        const int remain = R - i; // Count lines!
        if( remain > 0 ) {
          helper.draw(painter, data, i, remain);
        }
      }

      struct PointsHelper {
        static constexpr int POINTS = 32;

        PointsHelper() noexcept = default;

        void init(QPainter *painter)
        {
          constexpr qreal TWO = 2;

          const qreal sx = painter->transform().m11();
          const qreal sy = painter->transform().m22();
          const qreal  r = (painter->pen().widthF() + TWO)/TWO;
          _rx = r/sx;
          _ry = r/sy;

          painter->setBrush({painter->pen().color(), Qt::SolidPattern});
          painter->setPen(Qt::NoPen);
        }

        inline void draw(QPainter *painter, const IPlotSeriesData *data,
                         const int L, const int numPoints) const
        {
          data->values(_points, L, L + numPoints - 1);
          for(int i = 0; i < numPoints; i++) {
            painter->drawEllipse(_points[i], _rx, _ry);
          }
        }

      private:
        static QPointF _points[POINTS];
        qreal _rx{};
        qreal _ry{};
      };

      QPointF PointsHelper::_points[PointsHelper::POINTS];

      void drawPoints(QPainter *painter,
                      const IPlotSeriesData *data, const int L, const int R)
      {
        const int numData = R - L + 1;
        if( numData < 1 ) {
          return;
        }

        painter->save();

        PointsHelper helper;
        helper.init(painter);

        const int numBlocks = numData/PointsHelper::POINTS;
        for(int i = 0; i < numBlocks; i++) {
          helper.draw(painter, data,
                      L + i*PointsHelper::POINTS, PointsHelper::POINTS);
        }

        const int numRemain = numData%PointsHelper::POINTS;
        if( numRemain > 0 ) {
          helper.draw(painter, data,
                      L + numBlocks*PointsHelper::POINTS, numRemain);
        }

        painter->restore();
      }

      void drawSteps(QPainter *painter,
                     const IPlotSeriesData *data, const int L, const int R)
      {
        constexpr int STEPS = 32;
        QPointF points[2*STEPS+1];

        int i = L;
        points[0] = data->value(i);
        while( i + STEPS <= R ) {
          data->values(&points[1], i+1, i+STEPS);
          for(int j = STEPS; j > 0; j--) {
            points[2*j  ] = points[j];
            points[2*j-1] = QPointF(points[j].x(), points[j-1].y());
          }
          painter->drawPolyline(points, 2*STEPS+1);
          points[0] = points[2*STEPS];
          i += STEPS;
        }

        const int remain = R - i + 1; // Remaining Points!
        if( remain > 1 ) {
          data->values(&points[1], i+1, R);
          for(int j = remain-1; j > 0; j--) {
            points[2*j  ] = points[j];
            points[2*j-1] = QPointF(points[j].x(), points[j-1].y());
          }
          painter->drawPolyline(points, 2*remain-1);
        }
      }

    } // namespace impl_draw

    void frame(QPainter *painter,
               const QRectF& rect, const QPen& pen)
    {
      painter->setPen(pen);
      painter->drawRect(rect.adjusted(0.5, 0.5, -0.5, -0.5));
    }

    void gridX(QPainter *painter,
               const IAxisElement *xAxis, const QTransform& mapping,
               const QRectF& rect, const QPen& pen)
    {
      painter->setPen(pen);

      const AxisLabels xAxisLabels = xAxis->labels();
      for(const AxisLabel& label : xAxisLabels) {
        const qreal x = std::floor(mapping.map(QPointF(label.value(), 0)).x());
        const QPointF begin(rect.left() + x + 0.5, rect.top() + 0.5);
        const QPointF end(begin + QPointF(0, rect.height() - 1));
        painter->drawLine(begin, end);
      }
    }

    void gridY(QPainter *painter,
               const IAxisElement *yAxis, const QTransform& mapping,
               const QRectF& rect, const QPen& pen)
    {
      painter->setPen(pen);

      const AxisLabels yAxisLabels = yAxis->labels();
      for(const AxisLabel& label : yAxisLabels) {
        const qreal y = std::floor(mapping.map(QPointF(0, label.value())).y());
        const QPointF begin(rect.left() + 0.5, rect.top() + y + 0.5);
        const QPointF end(begin + QPointF(rect.width() - 1, 0));
        painter->drawLine(begin, end);
      }
    }

    void series(QPainter *painter,
                const QRectF& screen, const Series& theSeries,
                const PlotRange& viewX, const PlotRange& viewY,
                const DrawFlags flags)
    {
      if( theSeries.isEmpty() ) {
        return;
      }

      const IPlotSeriesData *data = theSeries.data();

      if( !viewX.isValid()  ||  !viewY.isValid()  ||  screen.isEmpty()  ||
          viewX.begin >= data->rangeX().end  ||
          viewX.end <= data->rangeX().begin ) {
        return;
      }

      QPen pen = PlotTheme::seriesPen(theSeries.color(),
                                      flags.testFlag(IsActive) ? 2.0 : 1.0);
      pen.setCosmetic(true);
      painter->setPen(pen);

      const QTransform xform =
          Mapping::viewToScreen(screen.size(), viewX, viewY) *
          QTransform::fromTranslate(screen.topLeft().x(), screen.topLeft().y());
      painter->setTransform(xform);

      const int L = data->findLeft(viewX.begin) >= 0
          ? data->findLeft(viewX.begin)
          : 0;
      const int R = data->findRight(viewX.end) >= 0
          ? data->findRight(viewX.end)
          : data->size() - 1;

      if( L >= R ) {
        return;
      }

      const int numData   = R - L + 1;
      const int numScreen = int(screen.width());

      const bool is_under = numScreen/numData >= 3;

      if( is_under  &&  flags.testFlag(Steps) ) {
        impl_draw::drawSteps(painter, data, L, R);
      } else {
        impl_draw::drawLines(painter, data, L, R);
      }

      if( is_under  &&  flags.testFlag(Marks) ) {
        impl_draw::drawPoints(painter, data, L, R);
      }
    }

  } // namespace Draw

} // namespace plot
