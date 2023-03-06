/****************************************************************************
** Copyright (c) 2023, Carsten Schmidt. All rights reserved.
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

#include <string>
#include <vector>

#include <cs/Core/csutil_config.h>

namespace cs {

  ////// Types ///////////////////////////////////////////////////////////////

  using ByteArray = std::vector<uint8_t>;

  ////// Implementation //////////////////////////////////////////////////////

  CS_UTIL_EXPORT ByteArray toByteArray(const char *str, const std::size_t lenStr);

  CS_UTIL_EXPORT std::string toString(const uint8_t *data, const std::size_t sizData,
                                      const char fill = '\0', const bool is_upper = false);

  ////// Helper //////////////////////////////////////////////////////////////

  inline ByteArray toByteArray(const std::string& s)
  {
    return toByteArray(s.data(), s.length());
  }

  inline std::string toString(const uint8_t *data, const std::size_t sizData,
                              const bool is_upper)
  {
    return toString(data, sizData, '\0', is_upper);
  }

  inline std::string toString(const ByteArray& a,
                              const char fill = '\0', const bool is_upper = false)
  {
    return toString(a.data(), a.size(), fill, is_upper);
  }

  inline std::string toString(const ByteArray& a,
                              const bool is_upper)
  {
    return toString(a, '\0', is_upper);
  }

} // namespace cs
