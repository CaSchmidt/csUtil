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

#include <cs/Core/csutil_config.h>

#include <cs/Core/CharUtil.h>

namespace cs {

  ////// Type Traits /////////////////////////////////////////////////////////

  template<typename T>
  using is_typeid_type = std::bool_constant<
  std::is_unsigned_v<T>  &&  (sizeof(T) > 2)
  >;

  template<typename T>
  inline constexpr bool is_typeid_type_v = is_typeid_type<T>::value;

  ////// Types ///////////////////////////////////////////////////////////////

  template<typename T>
  using declare_tokenid_type = std::enable_if_t<is_typeid_type_v<T>,T>;

  using tokenid_t = declare_tokenid_type<unsigned>;

  ////// Token IDs ///////////////////////////////////////////////////////////

  enum TokenId : tokenid_t {
    TOK_EndOfInput = 0x0,
    TOK_Unknown    = 0xFFFF, // <not a character>; cf. https://www.unicode.org/charts/PDF/UFFF0.pdf
    TOK_User       = 0x10000
  };

  constexpr tokenid_t make_tokenid(const tokenid_t user)
  {
    return static_cast<tokenid_t>(TOK_User) + user;
  }

  ////// Base Token Implementation ///////////////////////////////////////////

  using TokenPtr = std::unique_ptr<class BaseToken>;

  class CS_UTIL_EXPORT BaseToken {
  protected:
    struct ctor_tag {
      ctor_tag() noexcept = default;
    };

  public:
    BaseToken(const tokenid_t id, const size_t size,
              const ctor_tag& = ctor_tag()) noexcept;
    virtual ~BaseToken() noexcept;

    tokenid_t id() const;

    size_t column() const;
    size_t line() const;

    void setLocation(const size_t line, const size_t column);

    size_t size() const;

    static TokenPtr make(const tokenid_t id, const size_t size);

    inline static TokenPtr make(const TokenId id, const size_t size)
    {
      return make(static_cast<tokenid_t>(id), size);
    }

  private:
    BaseToken() noexcept = delete;

    size_t    _column{0};
    tokenid_t _id{};
    size_t    _line{0};
    size_t    _size{0};
  };

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

    inline static TokenPtr make(const TokenId id, const value_type& value, const size_t size)
    {
      return make(static_cast<tokenid_t>(id), value, size);
    }

  private:
    ValueToken() noexcept = delete;

    value_type _value;
  };

  ////// Utilities ///////////////////////////////////////////////////////////

  template<typename T>
  inline T as_value(const TokenPtr& tok, const T defValue = T())
  {
    return dynamic_cast<const ValueToken<T>*>(tok.get())
        ? dynamic_cast<const ValueToken<T>*>(tok.get())->value()
        : defValue;
  }

  template<typename CharT>
  inline bool is_literal(const TokenPtr& tok, const CharT ch)
  {
    return tok  &&  tok->id() == static_cast<tokenid_t>(ch);
  }

} // namespace cs
