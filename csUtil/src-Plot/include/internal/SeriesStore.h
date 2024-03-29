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

#include "internal/Scale.h"
#include "internal/Series.h"

namespace plot {

  class SeriesStore {
  public:
    SeriesStore();
    ~SeriesStore();

    bool contains(const QString& seriesName) const;

    QStringList names(const bool sorted = false) const;

    bool insert(const Series& series);
    bool remove(const QString& seriesName);

    PlotRange rangeX(const QString& seriesName) const;
    PlotRange rangeY(const QString& seriesName) const;

    PlotRange totalRangeX() const;

    Series& series(const QString& seriesName);
    const Series& series(const QString& seriesName) const;

  private:
    bool addToScales(const QString& seriesName);
    void removeFromScales(const QString& seriesName);

    bool isGroupedScale(const QString& seriesName, const QString& scaleName) const;

    QHash<QString,Scale> _scales;
    QHash<QString,Series> _series;

    static Series _nullSeries;
  };

} // namespace plot
