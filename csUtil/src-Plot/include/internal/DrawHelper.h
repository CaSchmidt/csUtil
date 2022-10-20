/****************************************************************************
** Copyright (c) 2022, Carsten Schmidt. All rights reserved.
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

#include <QtCore/QPointF>
#include <QtGui/QPainter>

#include <Plot/IPlotSeriesData.h>

namespace plot {

  namespace Draw {

    namespace impl_draw {

      ////// LinesHelper /////////////////////////////////////////////////////

      struct LinesHelper {
        static constexpr int LINES = 32;

        LinesHelper() noexcept;

        inline void draw(QPainter *painter, const IPlotSeriesData *data,
                         const int L, const int numLines) const
        {
          data->values(_points, L, L + numLines);
          painter->drawPolyline(_points, numLines + 1);
        }

      private:
        static QPointF _points[LINES + 1];
      };

      ////// PointsHelper ////////////////////////////////////////////////////

      struct PointsHelper {
        static constexpr int POINTS = 32;

        PointsHelper(QPainter *painter) noexcept;

        inline void draw(QPainter *painter, const IPlotSeriesData *data,
                         const int L, const int numPoints) const
        {
          data->values(_points, L, L + numPoints - 1);
          for(int i = 0; i < numPoints; i++) {
            painter->drawEllipse(_points[i], _rx, _ry);
          }
        }

      private:
        PointsHelper() noexcept = delete;

        static QPointF _points[POINTS];
        qreal _rx{};
        qreal _ry{};
      };

      ////// StepsHelper /////////////////////////////////////////////////////

      struct StepsHelper {
        static constexpr int STEPS = 32;

        StepsHelper() noexcept;

        inline void draw(QPainter *painter, const IPlotSeriesData *data,
                         const int L, const int numSteps) const
        {
          data->values(_points, L, L + numSteps);
          for(int i = STEPS; i > 0; i--) {
            _points[i*2  ] = _points[i];
            _points[i*2-1] = {_points[i].x(), _points[i-1].y()};
          }
          painter->drawPolyline(_points, numSteps*2 + 1);
        }

      private:
        static QPointF _points[STEPS*2 + 1];
      };

    } // namespace impl_draw

  } // namespace Draw

} // namespace plot
