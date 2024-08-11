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

#include <array>
#include <charconv>

#include "internal/AxisLabel.h"

#include "cs/Core/Constants.h"
#include "cs/Core/Container.h"
#include "cs/Core/QStringUtil.h"
#include "cs/Math/Math.h"
#include "cs/Text/StringUtil.h"
#include "cs/Text/StringValue.h"

namespace plot {

  AxisLabelValues computeLabelValues(const double min, const double max,
                                     const std::size_t numIntervals)
  {
    using  size_type = typename AxisLabelValues::size_type;
    using value_type = typename AxisLabelValues::value_type;
    using          k = cs::RealKonst<value_type>;
    using          m = cs::Math<value_type>;

    // (0) Sanity Check //////////////////////////////////////////////////////

    const value_type rawSpan = max - min;
    if( rawSpan <= k::ZERO  ||  numIntervals < 1 ) {
      return AxisLabelValues{};
    }

    // (1) Compute span to be a multiple of 10^x /////////////////////////////

    /*
     * floor(log10(  0.009)) = -3  ->  x10 =   0.001
     * floor(log10(  0.09 )) = -2  ->  x10 =   0.01
     * floor(log10(  0.9  )) = -1  ->  x10 =   0.1
     * floor(log10(  9    )) =  0  ->  x10 =   1
     * floor(log10( 99    )) =  1  ->  x10 =  10
     * floor(log10(999    )) =  2  ->  x10 = 100
     */
    const value_type      x10 = m::pow(10, m::floor(m::log10(rawSpan)));
    const value_type stepSpan = m::round(rawSpan/x10)*x10;

    // (2) Compute Step Size /////////////////////////////////////////////////

    const value_type step = stepSpan/value_type(numIntervals);

    // (3) Align Label Values with Step Size /////////////////////////////////

    const value_type startLabel = m::ceil( min/step)*step;
    const value_type   endLabel = m::floor(max/step)*step;
    const value_type  labelSpan = endLabel - startLabel;
    if( labelSpan <= k::ZERO ) {
      return AxisLabelValues{};
    }

    // (4) Generate Labels ///////////////////////////////////////////////////

    AxisLabelValues result;
    if( !cs::resize(result, size_type(m::intgr(labelSpan/step)) + 1) ) {
      return AxisLabelValues{};
    }
    for(size_type i = 0; i < result.size(); i++) {
      result[i] = startLabel + value_type(i)*step;
    }

    return result;
  }

  QString formatLabelValue(const double value, const PlotAxisLabelFormat& alf)
  {
    const char fmt = std::get<0>(alf);
    const int prec = std::get<1>(alf);

    std::string str = cs::toString(value, fmt, prec);
    cs::removeTrailingZeros(str);

    return cs::toQString(cs::toUtf8StringView(str));
  }

  AxisLabels formatLabelValues(const AxisLabelValues& values,
                               const PlotAxisLabelFormat& alf)
  {
    if( values.empty() ) {
      return AxisLabels{};
    }

    AxisLabels result;
    for(const double value : values) {
      result.emplace_back(formatLabelValue(value, alf), value);
    }

    return result;
  }

} // namespace plot
