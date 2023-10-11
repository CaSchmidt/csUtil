/****************************************************************************
** Copyright (c) 2020, Carsten Schmidt. All rights reserved.
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

#ifndef N4_MANIPULATOR_H
#define N4_MANIPULATOR_H

#include <N4/TypeTraits.h>

namespace n4 {

  class NoManipulator {
  public:
    NoManipulator(real_t *) noexcept
    {
    }

    ~NoManipulator() noexcept = default;

  private:
    NoManipulator() noexcept = delete;
  };

  template<size_t index>
  class VectorProperty {
  public:
    VectorProperty(real_t *base) noexcept
      : _data(base + index)
    {
    }

    ~VectorProperty() noexcept = default;

    inline operator real_t() const
    {
      return *_data;
    }

    inline real_t operator=(const real_t value)
    {
      *_data = value;
      return *_data;
    }

    inline real_t operator+=(const real_t value)
    {
      *_data += value;
      return *_data;
    }

    inline real_t operator-=(const real_t value)
    {
      *_data -= value;
      return *_data;
    }

    inline real_t operator*=(const real_t value)
    {
      *_data *= value;
      return *_data;
    }

    inline real_t operator/=(const real_t value)
    {
      *_data /= value;
      return *_data;
    }

  private:
    VectorProperty() noexcept = delete;

    real_t *_data{nullptr};
  };

} // namespace n4

#endif // N4_MANIPULATOR_H
