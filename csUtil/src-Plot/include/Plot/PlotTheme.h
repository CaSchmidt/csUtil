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

#include <array>
#include <vector>

#include <QtGui/QBrush>
#include <QtGui/QColor>
#include <QtGui/QPen>

#include <cs/Core/csutil_config.h>

namespace plot {

  class CS_UTIL_EXPORT PlotTheme {
  public:
    using size_type = std::size_t;

    enum DrawElement : size_type {
      Background = 0,
      Frame,
      Grid,
      RubberBand,
      Series,
      Text,
      Num_DrawElements
    };

    PlotTheme();
    ~PlotTheme();

    void clear();

    QColor color(const DrawElement e) const;
    bool setColor(const DrawElement e, const QColor& c);

    QBrush brush(const DrawElement e) const;
    QPen pen(const DrawElement e) const;

    QPen seriesPen(const QColor& c, const qreal w) const;
    QPen yAxisPen(const QColor& c) const;

    static bool isEmptyUnit(const QString& unit);
    static QString cleanUnit(const QString& unit);

    static QString titleString(const QString& name, const QString& unit);

  private:
    std::array<QColor,Num_DrawElements> _colors{};
    size_type _seriesColorIndex{0};
    std::vector<QColor> _seriesColors{};
  };

  PlotTheme makeTextbookTheme();
  PlotTheme makeOscilloscopeTheme();

} // namespace plot
