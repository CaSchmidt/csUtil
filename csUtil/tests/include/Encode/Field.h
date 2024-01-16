/****************************************************************************
** Copyright (c) 2024, Carsten Schmidt. All rights reserved.
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

#include <memory>
#include <string>

#include <cs/Core/Bit.h>

namespace Encode {

  using size_t = std::size_t;

  static_assert( std::is_unsigned_v<size_t> );

  template<typename T>
  class IField {
  public:
    using value_type = T;

    virtual ~IField() noexcept
    {
    }

    virtual bool isValid() const
    {
      constexpr size_t MAX_BIT = sizeof(value_type)*8 - 1;

      return _from <= _to  &&  _at + _to - _from <= MAX_BIT;
    }

    virtual std::string name() const
    {
      return std::string();
    }

    virtual value_type value(const value_type x) const
    {
      const T mask = cs::makeBitMask<T>(_to - _from + 1);

      return ((x >> _from) & mask) << _at;
    }

  protected:
    IField(const size_t from, const size_t to,
           const size_t at) noexcept
      : _at{at}
      , _from{from}
      , _to{to}
    {
    }

  private:
    IField() noexcept = delete;

    size_t _at{0};
    size_t _from{0};
    size_t _to{0};
  };

  template<typename T>
  using FieldPtr = std::unique_ptr<IField<T>>;

} // namespace Encode
