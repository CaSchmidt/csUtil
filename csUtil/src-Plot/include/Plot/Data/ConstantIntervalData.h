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

#include <vector>

#include <cs/Core/Concepts.h>
#include <cs/Core/Constants.h>

#include <Plot/IPlotSeriesData.h>

namespace plot {

  template<typename T> requires cs::IsReal<T>
  class ConstantIntervalData : public IPlotSeriesData {
  private:
    struct ctor_tag {
      ctor_tag() noexcept
      {
      }
    };

  public:
    using vector_type = std::vector<T>;
    using   size_type = typename vector_type::size_type;
    using  value_type = typename vector_type::value_type;

    ConstantIntervalData(const QString& name, const QString& unit,
                         const value_type xStart, const value_type xInter,
                         const ctor_tag& = ctor_tag{}) noexcept
      : IPlotSeriesData{name, unit, true}
      , _xStart{xStart}
      , _xInter{xInter}
    {
    }

    ~ConstantIntervalData() noexcept
    {
    }

    static PlotSeriesDataPtr make(const QString& name, const QString& unit,
                                  const value_type xStart, const value_type xInter,
                                  const vector_type& y)
    {
      PlotSeriesDataPtr result = make_init(name, unit, xStart, xInter, y);

      if( result ) {
        ConstantIntervalData *thiz = dynamic_cast<ConstantIntervalData*>(result.get());

        try {
          thiz->_y = y;
        } catch(...) {
          return PlotSeriesDataPtr{};
        }

        thiz->updateRange();
      }

      return result;
    }

    // Interface /////////////////////////////////////////////////////////////

    QPointF value(const int i) const
    {
      return QPointF{getX(i), getY(i)};
    }

    int size() const
    {
      return int(_y.size());
    }

    qreal valueX(const int i) const
    {
      return getX(i);
    }

    qreal valueY(const int i) const
    {
      return getY(i);
    }

    void values(QPointF *points, const int L, const int R) const
    {
      for(int i = L; i <= R; i++) {
        *points++ = QPointF{getX(i), getY(i)};
      }
    }

  private:
    using k = cs::konst<value_type>;

    inline qreal getX(const int i) const
    {
      return qreal(_xStart + value_type(i)*_xInter);
    }

    inline qreal getY(const int i) const
    {
      return qreal(_y[size_type(i)]);
    }

    static PlotSeriesDataPtr make_init(const QString& name, const QString& unit,
                                       const value_type xStart, const value_type xInter,
                                       const vector_type& y)
    {
      constexpr size_type TWO = 2;

      if( name.isEmpty()  ||  xInter <= k::ZERO  ||  y.size() < TWO ) {
        return PlotSeriesDataPtr{};
      }

      return std::make_unique<ConstantIntervalData>(name, unit, xStart, xInter);
    }

    value_type  _xStart{};
    value_type  _xInter{};
    vector_type _y{};
  };

} // namespace plot
