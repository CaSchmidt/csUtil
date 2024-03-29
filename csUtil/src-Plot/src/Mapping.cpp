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

#include "internal/Mapping.h"

namespace plot {

  ////// Public //////////////////////////////////////////////////////////////

  namespace Mapping {

    QTransform viewToScreen(const QSizeF& screen,
                            const PlotRange& viewX, const PlotRange& viewY)
    {
      QTransform result;

      if( screen.isEmpty()  ||  !viewX.isValid()  ||  !viewY.isValid() ) {
        return result;
      }

      const qreal dx = -viewX.begin;
      const qreal dy = -viewY.begin;

      const qreal sx = (screen.width()  - 1)/viewX.range();
      const qreal sy = (screen.height() - 1)/viewY.range();

      result *= QTransform::fromTranslate(dx, dy);
      result *= QTransform::fromScale(sx, sy);
      result *= QTransform(1,  0,
                           0, -1,
                           0.5, screen.height() - 0.5);

      return result;
    }

    QTransform screenToView(const QSizeF& screen,
                            const PlotRange& viewX, const PlotRange& viewY,
                            const bool isRelative)
    {
      QTransform result;

      if( screen.isEmpty()  ||  !viewX.isValid()  ||  !viewY.isValid() ) {
        return result;
      }

      const qreal sx = viewX.range()/(screen.width()  - 1);
      const qreal sy = viewY.range()/(screen.height() - 1);

      const qreal dx = viewX.begin;
      const qreal dy = viewY.begin;

      if( !isRelative ) {
        result *= QTransform(1,  0,
                             0, -1,
                             0, screen.height() - 1);
      }
      result *= QTransform::fromScale(sx, sy);
      if( !isRelative ) {
        result *= QTransform::fromTranslate(dx, dy);
      }

      return result;
    }

  } // namespace Mapping

} // namespace plot
