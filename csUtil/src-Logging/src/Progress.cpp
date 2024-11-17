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
#include <print>

#include "cs/Logging/Progress.h"

#include "cs/Math/Numeric.h"

namespace cs {

  ////// public //////////////////////////////////////////////////////////////

  Progress::Progress(const std::size_t step, FILE *file, const bool owner, const ctor_tag&)
    : _file{file}
    , _is_owner{owner}
  {
    _step = step > 0
        ? step
        : DEFAULT_STEP;
  }

  Progress::~Progress()
  {
    if( _file == stderr  ||  _file == stdin  ||  _file == stdout ) {
      return;
    }
    if( _is_owner ) {
      fclose(_file);
    }
  }

  void Progress::progressFlush() const
  {
    fflush(_file);
  }

  void Progress::setProgressRange(const std::size_t min, const std::size_t max) const
  {
    if( min < max ) {
      const_cast<Progress*>(this)->_min = min;
      const_cast<Progress*>(this)->_max = max;
    }
  }

  void Progress::setProgressValue(const std::size_t val) const
  {
    constexpr std::size_t HUNDRED = 100;
    constexpr std::size_t    ZERO =   0;

    const std::size_t cval = std::clamp(val, _min, _max);

    const std::size_t  pos = cval - _min; // position in span
    const std::size_t span = _max - _min;

    if( pos%_step == ZERO  ||  pos == span ) {
      const std::size_t   pct = (pos*HUNDRED)/span;
      const std::size_t width = countDigits(span);

      std::println(_file, "Progress: {0:3}% ({1:{3}}/{2})",
                   pct, pos, span, width);
    }
  }

  ProgressPtr Progress::make(const std::size_t step, FILE *file, const bool is_owner)
  {
    return std::make_shared<Progress>(step, file, is_owner);
  }

} // namespace cs
