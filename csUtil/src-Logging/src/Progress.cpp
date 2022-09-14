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

#include <algorithm>

#include "cs/Logging/Progress.h"

#include "cs/Math/Numeric.h"

namespace cs {

  ////// public //////////////////////////////////////////////////////////////

  Progress::Progress(FILE *file, const bool owner)
    : _file{file}
    , _owner{owner}
  {
  }

  Progress::Progress(int step, FILE *file, const bool owner)
    : _file{file}
    , _owner{owner}
  {
    _step = step > 0
        ? step
        : DEFAULT_STEP;
  }

  Progress::~Progress()
  {
    if( _owner  &&  _file != stderr  &&  _file != stdout ) {
      fclose(_file);
    }
  }

  void Progress::progressFlush() const
  {
    fflush(_file);
  }

  void Progress::setProgressMaximum(const int max) const
  {
    if( _min < max ) {
      const_cast<Progress*>(this)->_max = max;
    }
  }

  void Progress::setProgressMinimum(const int min) const
  {
    if( min < _max ) {
      const_cast<Progress*>(this)->_min = min;
    }
  }

  void Progress::setProgressRange(const int min, const int max) const
  {
    if( min < max ) {
      const_cast<Progress*>(this)->_min = min;
      const_cast<Progress*>(this)->_max = max;
    }
  }

  void Progress::setProgressValue(const int val) const
  {
    constexpr int HUNDRED = 100;
    constexpr int    ZERO =   0;

    const int _val = std::clamp<int>(val, _min, _max);

    const int  pos = _val - _min;
    const int span = _max - _min;

    if( pos%_step == ZERO  ||  pos == span ) {
      const int   pct = (pos*HUNDRED)/span;
      const int width = int(countDigits<int>(span));

      fprintf(_file, "Progress: %3d%% (%*d/%d)\n", pct, width, pos, span);
    }
  }

} // namespace cs
