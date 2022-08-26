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

  template<typename T>
  struct is_flags_enabled : std::false_type {};

  template<typename T>
  inline constexpr bool is_flags_enabled_v = is_flags_enabled<T>::value;

  template<typename T>
  using is_flags = std::bool_constant<
  std::is_enum_v<T>                              &&
  std::is_unsigned_v<safe_underlying_type_t<T>>  &&
  is_flags_enabled_v<T>
  >;

  template<typename T>
  inline constexpr bool is_flags_v = is_flags<T>::value;

  template<typename T>
  using if_flags_t = std::enable_if_t<is_flags_v<T>,T>;

  template<typename T>
  using if_flags_bool = std::enable_if_t<is_flags_v<T>,bool>;

} // namespace cs

// AND ///////////////////////////////////////////////////////////////////////

template<typename T>
inline cs::if_flags_t<T> operator&(const T& a, const T& b)
{
  using value_type = cs::safe_underlying_type_t<T>;
  return static_cast<T>(static_cast<value_type>(a) & static_cast<value_type>(b));
}

template<typename T>
inline cs::if_flags_t<T>& operator&=(T& result, const T& a)
{
  using value_type = cs::safe_underlying_type_t<T>;
  result = static_cast<T>(static_cast<value_type>(result) & static_cast<value_type>(a));
  return result;
}

// NOT ///////////////////////////////////////////////////////////////////////

template<typename T>
inline cs::if_flags_t<T> operator~(const T& a)
{
  using value_type = cs::safe_underlying_type_t<T>;
  return static_cast<T>(~static_cast<value_type>(a));
}

// OR ////////////////////////////////////////////////////////////////////////

template<typename T>
inline cs::if_flags_t<T> operator|(const T& a, const T& b)
{
  using value_type = cs::safe_underlying_type_t<T>;
  return static_cast<T>(static_cast<value_type>(a) | static_cast<value_type>(b));
}

template<typename T>
inline cs::if_flags_t<T>& operator|=(T& result, const T& a)
{
  using value_type = cs::safe_underlying_type_t<T>;
  result = static_cast<T>(static_cast<value_type>(result) | static_cast<value_type>(a));
  return result;
}

// XOR ///////////////////////////////////////////////////////////////////////

template<typename T>
inline cs::if_flags_t<T> operator^(const T& a, const T& b)
{
  using value_type = cs::safe_underlying_type_t<T>;
  return static_cast<T>(static_cast<value_type>(a) ^ static_cast<value_type>(b));
}

template<typename T>
inline cs::if_flags_t<T>& operator^=(T& result, const T& a)
{
  using value_type = cs::safe_underlying_type_t<T>;
  result = static_cast<T>(static_cast<value_type>(result) ^ static_cast<value_type>(a));
  return result;
}

// Implementation ////////////////////////////////////////////////////////////

namespace cs {

  template<typename EnumT>
  class Flags {
  public:
    using  enum_type = if_flags_t<EnumT>;
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

    Flags(const value_type value) noexcept
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
      return static_cast<value_type>(_flags) != 0;
    }

    inline operator enum_type() const
    {
      return _flags;
    }

    inline operator value_type() const
    {
      return static_cast<value_type>(_flags);
    }

    inline bool testFlag(const enum_type f) const
    {
      return static_cast<value_type>(_flags & f) != 0;
    }

    inline bool testMask(const enum_type m) const
    {
      return (_flags & m) == m;
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

  private:
    enum_type _flags{};
  };

} // namespace cs

// Macros ////////////////////////////////////////////////////////////////////

#define CS_ENABLE_FLAGS(T)                           \
  namespace cs {                                     \
    template<>                                       \
    struct is_flags_enabled<T> : std::true_type {};  \
  }
