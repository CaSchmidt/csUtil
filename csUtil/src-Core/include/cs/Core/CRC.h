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

#pragma once

#include <cstdint>

#include <array>
#include <limits>
#include <type_traits>

/*
 * NOTE:
 *
 * For a start on CRC computation, cf.:
 * https://en.wikipedia.org/wiki/Cyclic_redundancy_check
 *
 * For a table-driven CRC-32 implementation, cf.:
 * [MS-ABS] Address Book File Structure, 5.1 32-Bit CRC Algorithm
 *
 * This implementation closely follows:
 * Reversing CRC - Theory and Practice, M. Stigge & H. Plötz & W. Müller & J.-P. Redlich
 *
 * For test cases, cf.:
 * https://reveng.sourceforge.io/
 * Specification of CRC Routines, Document ID 16, AUTOSAR CP R21-11
 */

namespace cs {

  namespace impl_crc {

    template<typename T>
    using is_crc_type = std::bool_constant<
    std::is_unsigned_v<T>
    >;

    template<typename T>
    inline constexpr bool is_crc_type_v = is_crc_type<T>::value;

    template<typename T>
    using if_crc_t = std::enable_if_t<is_crc_type_v<T>,T>;

    template<typename T, std::size_t BITS>
    using is_shift = std::bool_constant<
    0 < BITS  &&  BITS < sizeof(T)*8
    >;

    template<typename T, std::size_t BITS>
    inline constexpr bool is_shift_v = is_shift<T,BITS>::value;

  } // namespace impl_crc

  template<typename T,
           T _XOR_VALUE = std::numeric_limits<T>::max()>
  class CRC {
  public:
    using  byte_type = uint8_t;
    using  size_type = std::size_t;
    using value_type = impl_crc::if_crc_t<T>;

    static constexpr value_type XOR_VALUE = _XOR_VALUE;

    CRC(const value_type crcpoly, const bool is_reversed = false) noexcept
      : _crcpoly{crcpoly}
    {
      if( !is_reversed ) {
        _crcpoly = reverseBits(crcpoly);
      }
      initializeTable();
    }

    ~CRC() noexcept
    {
    }

    value_type operator()(const byte_type *buffer, const size_type count,
                          value_type sum = 0) const
    {
      sum ^= XOR_VALUE;

      for(size_type i = 0; i < count; i++) {
        const size_type index =
            size_type(bitsShiftedOut<M>(sum)) ^ size_type(buffer[i]);
        sum = shiftLeft<M>(sum);
        sum ^= _crctable[index];
      }

      return sum ^ XOR_VALUE;
    }

  private:
    static constexpr value_type  ONE = 1;
    static constexpr value_type ZERO = 0;

    static constexpr size_type M = sizeof(byte_type)*8;
    static constexpr size_type N = sizeof(value_type)*8;

    static constexpr size_type Mcount = size_type(1) << M;

    CRC() noexcept = delete;

    template<size_type BITS = 1>
    inline static value_type bitsShiftedOut(const value_type& in)
    {
      static_assert( impl_crc::is_shift_v<value_type,BITS> );

      constexpr value_type MASK = (ONE << BITS) - ONE;

      return in & MASK;
    }

    void initializeTable()
    {
      for(size_type index = 0; index < _crctable.size(); index++) {
        value_type crcreg = value_type(index);
        for(size_type k = 0; k < M; k++) {
          const value_type bits_just_shifted_out = bitsShiftedOut(crcreg);
          crcreg = shiftLeft(crcreg);
          if( bits_just_shifted_out != ZERO ) {
            crcreg ^= _crcpoly;
          }
        }
        _crctable[index] = crcreg;
      }
    }

    inline static value_type reverseBits(const value_type& in)
    {
      constexpr size_type MAX_BIT = sizeof(value_type)*8 - 1;

      value_type out = 0;

      for(size_type i = 0; i <= MAX_BIT; i++) {
        if( (in & (ONE << i)) != ZERO ) {
          out |= ONE << (MAX_BIT - i);
        }
      }

      return out;
    }

    template<size_type BITS = 1>
    inline static value_type shiftLeft(const value_type& in)
    {
      static_assert( impl_crc::is_shift_v<value_type,BITS> );

      return in >> BITS;
    }

    value_type _crcpoly{};
    std::array<value_type,Mcount> _crctable{};
  };

  using CRC8  = CRC<uint8_t>;
  using CRC16 = CRC<uint16_t>;
  using CRC32 = CRC<uint32_t>;
  using CRC64 = CRC<uint64_t>;

} // namespace cs
