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

#include <cs/Core/TypeTraits.h>

namespace cs {

  // Boolean Types ///////////////////////////////////////////////////////////

  template<typename T>
  concept IsBoolean = is_boolean_v<T>;

  // Arithmetic Types ////////////////////////////////////////////////////////

  template<typename T>
  concept IsArithmetic = is_integral_v<T>  ||  is_real_v<T>;

  template<typename T>
  concept IsIntegral = is_integral_v<T>;

  template<typename T>
  concept IsReal = is_real_v<T>;

  template<typename T>
  concept IsSigned = is_signed_v<T>;

  template<typename T>
  concept IsUnsigned = is_unsigned_v<T>;

  // Character Types /////////////////////////////////////////////////////////

  template<typename T>
  concept IsCharacter = is_char_v<T>;

  template<typename T>
  concept IsNarrowCharacter = is_narrowchar_v<T>;

  template<typename T>
  concept IsWideCharacter = is_widechar_v<T>;

  // Pointer Types ///////////////////////////////////////////////////////////

  template<typename T>
  concept IsPointer = std::is_pointer_v<T>;

  // Generic Types ///////////////////////////////////////////////////////////

  template<typename T>
  concept IsPow2Size = is_pow2size_v<T>;

} // namespace cs
