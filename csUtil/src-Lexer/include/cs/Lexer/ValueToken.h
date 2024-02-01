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

#include <cs/Lexer/Token.h>

namespace cs {

  ////// Value Token Implementation //////////////////////////////////////////

  template<typename T>
  class ValueToken : public BaseToken {
  protected:
    struct ctor_tag {
      ctor_tag() noexcept = default;
    };

  public:
    using value_type = T;

    ValueToken(const tokenid_t id, const value_type& value, const size_t size,
               const ctor_tag& = ctor_tag()) noexcept
      : BaseToken(id, size)
      , _value(value)
    {
    }

    ~ValueToken() noexcept
    {
    }

    value_type value() const
    {
      return _value;
    }

    static TokenPtr make(const tokenid_t id, const value_type& value, const size_t size)
    {
      return std::make_unique<ValueToken<value_type>>(id, value, size);
    }

  private:
    ValueToken() noexcept = delete;

    value_type _value;
  };

  ////// Utilities ///////////////////////////////////////////////////////////

  template<typename T>
  inline T to_value(const TokenPtr& tok, const T& defValue = T())
  {
    using VT = ValueToken<T>;

    return dynamic_cast<const VT*>(tok.get()) != nullptr
        ? dynamic_cast<const VT*>(tok.get())->value()
        : defValue;
  }

} // namespace cs
