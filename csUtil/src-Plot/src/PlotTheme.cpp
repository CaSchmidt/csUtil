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

namespace plot {

  ////// public //////////////////////////////////////////////////////////////

  PlotTheme::PlotTheme()
  {
    clear();
  }

  PlotTheme::~PlotTheme()
  {
  }

  void PlotTheme::clear()
  {
    _colors.fill(Qt::black);
    _colors[Background] = Qt::white;

    _seriesColorIndex = 0;
    _seriesColors.clear();
  }

  QColor PlotTheme::color(const DrawElement e) const
  {
    const size_type index = size_type(e);
    if( index < 0  ||  index >= Num_DrawElements ) {
      return Qt::black;
    }

    if( e == Series  &&  !_seriesColors.empty() ) {
      PlotTheme *thiz = const_cast<PlotTheme*>(this);

      const QColor result = _seriesColors[_seriesColorIndex];
      thiz->_seriesColorIndex += 1;
      thiz->_seriesColorIndex %= _seriesColors.size();
      return result;
    }

    return _colors[index];
  }

  bool PlotTheme::setColor(const DrawElement e, const QColor& c)
  {
    const size_type index = size_type(e);
    if( index < 0  ||  index >= Num_DrawElements ) {
      return false;
    }

    if( e == Series ) {
      try {
        _seriesColors.push_back(c);
      } catch(...) {
        _seriesColors.clear();
        return false;
      }
    } else {
      _colors[index] = c;
    }

    return true;
  }

  QBrush PlotTheme::brush(const DrawElement e) const
  {
    return e != Series
        ? QBrush{color(e), Qt::SolidPattern}
        : QBrush{_colors[Series], Qt::SolidPattern};
  }

  QPen PlotTheme::pen(const DrawElement e) const
  {
    QPen pen{brush(e), 0};

    if(        e == Frame ) {
      pen.setWidthF(1);
      pen.setStyle(Qt::SolidLine);
      pen.setCapStyle(Qt::SquareCap);
      pen.setJoinStyle(Qt::MiterJoin);
    } else if( e == Grid ) {
      pen.setWidthF(1);
      pen.setStyle(Qt::DotLine);
      pen.setCapStyle(Qt::RoundCap);
      pen.setJoinStyle(Qt::RoundJoin);
    } else if( e == RubberBand ) {
      pen.setWidthF(1);
      pen.setStyle(Qt::DotLine);
      pen.setCapStyle(Qt::RoundCap);
      pen.setJoinStyle(Qt::RoundJoin);
    }

    return pen;
  }

  QPen PlotTheme::seriesPen(const QColor& c, const qreal w) const
  {
    return QPen{
      QBrush{c, Qt::SolidPattern}, w, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin
    };
  }

  QPen PlotTheme::yAxisPen(const QColor& c) const
  {
    QPen p = pen(Text);
    p.setColor(c);
    return p;
  }

  ////// public static ///////////////////////////////////////////////////////

  bool PlotTheme::isEmptyUnit(const QString& unit)
  {
    for(const QChar& c : unit) {
      if( !c.isSpace()  &&  c != QLatin1Char('-') ) {
        return false;
      }
    }
    return true;
  }

  QString PlotTheme::cleanUnit(const QString& unit)
  {
    if( isEmptyUnit(unit) ) {
      return QStringLiteral("-");
    }
    return unit.trimmed();
  }

  QString PlotTheme::titleString(const QString& name, const QString& unit)
  {
    return QStringLiteral("%1 [%2]").arg(name, cleanUnit(unit));
  }

  ////// Public //////////////////////////////////////////////////////////////

  PlotTheme makeTextbookTheme()
  {
    PlotTheme result;

    result.setColor(PlotTheme::Background, Qt::white);
    result.setColor(PlotTheme::Frame,      Qt::black);
    result.setColor(PlotTheme::Grid,       Qt::black);
    result.setColor(PlotTheme::RubberBand, Qt::black);
    result.setColor(PlotTheme::Text,       Qt::black);

    result.setColor(PlotTheme::Series, Qt::blue);
    result.setColor(PlotTheme::Series, Qt::green);
    result.setColor(PlotTheme::Series, Qt::red);
    result.setColor(PlotTheme::Series, Qt::cyan);
    result.setColor(PlotTheme::Series, Qt::magenta);
    result.setColor(PlotTheme::Series, Qt::yellow);
    result.setColor(PlotTheme::Series, Qt::black);

    return result;
  }

  PlotTheme makeOscilloscopeTheme()
  {
    PlotTheme result;

    result.setColor(PlotTheme::Background, Qt::black);
    result.setColor(PlotTheme::Frame,      Qt::white);
    result.setColor(PlotTheme::Grid,       Qt::white);
    result.setColor(PlotTheme::RubberBand, Qt::white);
    result.setColor(PlotTheme::Text,       Qt::white);

    result.setColor(PlotTheme::Series, Qt::blue);
    result.setColor(PlotTheme::Series, Qt::green);
    result.setColor(PlotTheme::Series, Qt::red);
    result.setColor(PlotTheme::Series, Qt::cyan);
    result.setColor(PlotTheme::Series, Qt::magenta);
    result.setColor(PlotTheme::Series, Qt::yellow);
    result.setColor(PlotTheme::Series, Qt::white);

    return result;
  }

} // namespace plot
