/****************************************************************************
** Copyright (c) 2018, Carsten Schmidt. All rights reserved.
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

#include <cs/Core/TypeTraits.h>

/*
 * NOTE:
 * This implementation is heavily inspired by Anthony Williams' article
 * "Using Enum Classes as Bitfields".
 * cf. https://accu.org/var/uploads/journals/Overload132.pdf#page=23
 */

namespace cs {

  template<typename EnumT>
  struct is_flags_enabled : std::false_type {};

  template<typename EnumT>
  inline constexpr bool is_flags_enabled_v = is_flags_enabled<EnumT>::value;

  template<typename EnumT>
  using is_flags = std::bool_constant<
  std::is_enum_v<EnumT>                              &&
  std::is_unsigned_v<safe_underlying_type_t<EnumT>>  &&
  is_flags_enabled_v<EnumT>
  >;

  template<typename EnumT>
  inline constexpr bool is_flags_v = is_flags<EnumT>::value;

  template<typename EnumT>
  concept IsFlags = is_flags_v<EnumT>;

} // namespace cs

// AND ///////////////////////////////////////////////////////////////////////

template<typename EnumT>
requires cs::IsFlags<EnumT>
constexpr EnumT operator&(const EnumT& a, const EnumT& b)
{
  using value_type = cs::safe_underlying_type_t<EnumT>;
  return static_cast<EnumT>(static_cast<value_type>(a) & static_cast<value_type>(b));
}

template<typename EnumT>
requires cs::IsFlags<EnumT>
constexpr EnumT& operator&=(EnumT& result, const EnumT& a)
{
  using value_type = cs::safe_underlying_type_t<EnumT>;
  result = static_cast<EnumT>(static_cast<value_type>(result) & static_cast<value_type>(a));
  return result;
}

// NOT ///////////////////////////////////////////////////////////////////////

template<typename EnumT>
requires cs::IsFlags<EnumT>
constexpr EnumT operator~(const EnumT& a)
{
  using value_type = cs::safe_underlying_type_t<EnumT>;
  return static_cast<EnumT>(~static_cast<value_type>(a));
}

// OR ////////////////////////////////////////////////////////////////////////

template<typename EnumT>
requires cs::IsFlags<EnumT>
constexpr EnumT operator|(const EnumT& a, const EnumT& b)
{
  using value_type = cs::safe_underlying_type_t<EnumT>;
  return static_cast<EnumT>(static_cast<value_type>(a) | static_cast<value_type>(b));
}

template<typename EnumT>
requires cs::IsFlags<EnumT>
constexpr EnumT& operator|=(EnumT& result, const EnumT& a)
{
  using value_type = cs::safe_underlying_type_t<EnumT>;
  result = static_cast<EnumT>(static_cast<value_type>(result) | static_cast<value_type>(a));
  return result;
}

// XOR ///////////////////////////////////////////////////////////////////////

template<typename EnumT>
requires cs::IsFlags<EnumT>
constexpr EnumT operator^(const EnumT& a, const EnumT& b)
{
  using value_type = cs::safe_underlying_type_t<EnumT>;
  return static_cast<EnumT>(static_cast<value_type>(a) ^ static_cast<value_type>(b));
}

template<typename EnumT>
requires cs::IsFlags<EnumT>
constexpr EnumT& operator^=(EnumT& result, const EnumT& a)
{
  using value_type = cs::safe_underlying_type_t<EnumT>;
  result = static_cast<EnumT>(static_cast<value_type>(result) ^ static_cast<value_type>(a));
  return result;
}

// Implementation ////////////////////////////////////////////////////////////

namespace cs {

  template<typename EnumT>
  requires IsFlags<EnumT>
  class Flags {
  public:
    using  enum_type = EnumT;
    using value_type = safe_underlying_type_t<enum_type>;

    Flags() noexcept = default;
    ~Flags() noexcept = default;

    Flags(const Flags&) noexcept = default;
    Flags& operator=(const Flags&) noexcept = default;

    Flags(Flags&&) noexcept = default;
    Flags& operator=(Flags&&) noexcept = default;

    Flags(const enum_type flags) noexcept
      : _flags{flags}
    {
    }

    explicit Flags(const value_type value) noexcept
      : _flags{static_cast<enum_type>(value)}
    {
    }

    Flags& operator=(const enum_type flags)
    {
      _flags = flags;
      return *this;
    }

    Flags& operator=(const value_type value)
    {
      _flags = static_cast<enum_type>(value);
      return *this;
    }

    inline operator bool() const
    {
      return static_cast<value_type>(_flags) != ZERO;
    }

    inline operator enum_type() const
    {
      return _flags;
    }

    inline operator value_type() const
    {
      return static_cast<value_type>(_flags);
    }

    inline bool testAll(const enum_type m) const
    {
      return (_flags & m) == m;
    }

    inline bool testAny(const enum_type f) const
    {
      return static_cast<value_type>(_flags & f) != ZERO;
    }

    inline void set(const enum_type f, const bool on = true)
    {
      if( on ) {
        _flags |=  f;
      } else {
        _flags &= ~f;
      }
    }

    inline void reset(const enum_type f)
    {
      set(f, false);
    }

    inline void toggle(const enum_type f)
    {
      _flags ^= f;
    }

  private:
    static constexpr value_type ZERO = 0;

    enum_type _flags{};
  };

} // namespace cs

// Macros ////////////////////////////////////////////////////////////////////

#define CS_ENABLE_FLAGS(EnumT)                           \
  namespace cs {                                         \
    template<>                                           \
    struct is_flags_enabled<EnumT> : std::true_type {};  \
  }
