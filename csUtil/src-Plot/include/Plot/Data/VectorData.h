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

#include <Plot/IPlotSeriesData.h>

namespace plot {

  template<typename T>
  class VectorData : public IPlotSeriesData {
  private:
    struct ctor_tag {
      ctor_tag() noexcept
      {
      }
    };

  public:
    VectorData(const QString& name, const QString& unit, const bool is_const_interval,
               const ctor_tag& = ctor_tag{}) noexcept
      : IPlotSeriesData{is_const_interval}
      , _name{name}
      , _unit{unit}
    {
    }

    ~VectorData() noexcept
    {
    }

    static PlotSeriesDataPtr make(const QString& name, const QString& unit,
                                  const std::vector<T>& x, const std::vector<T>& y,
                                  const bool is_const_interval = false)
    {
      PlotSeriesDataPtr result = make_init(name, unit, x, y, is_const_interval);

      if( result ) {
        VectorData *thiz = dynamic_cast<VectorData*>(result.get());

        try {
          thiz->_x = x;
          thiz->_y = y;
        } catch(...) {
          return PlotSeriesDataPtr{};
        }

        thiz->updateRange();
      }

      return result;
    }

    // Interface /////////////////////////////////////////////////////////////

    QString name() const
    {
      return _name;
    }

    QString unit() const
    {
      return _unit;
    }

    QPointF value(const int i) const
    {
      return QPointF{get(_x, i), get(_y, i)};
    }

    int size() const
    {
      return int(_x.size());
    }

    qreal valueX(const int i) const
    {
      return get(_x, i);
    }

    qreal valueY(const int i) const
    {
      return get(_y, i);
    }

    void values(QPointF *points, const int L, const int R) const
    {
      for(int i = L; i <= R; i++) {
        *points++ = QPointF{get(_x, i), get(_y, i)};
      }
    }

  private:
    using size_type = typename std::vector<T>::size_type;

    inline qreal get(const std::vector<T>& v, const int i) const
    {
      return qreal(v[size_type(i)]);
    }

    static PlotSeriesDataPtr make_init(const QString& name, const QString& unit,
                                       const std::vector<T>& x,
                                       const std::vector<T>& y,
                                       const bool is_const_interval)
    {
      constexpr size_type TWO = 2;

      if( name.isEmpty()  ||  x.size() < TWO  ||  x.size() != y.size() ) {
        return PlotSeriesDataPtr{};
      }

      return std::make_unique<VectorData>(name, unit, is_const_interval);
    }

    QString _name{};
    QString _unit{};
    std::vector<T> _x{};
    std::vector<T> _y{};
  };

} // namespace plot
