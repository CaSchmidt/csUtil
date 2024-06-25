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

#include <filesystem>
#include <istream>
#include <list>

#include <cs/Core/csutil_config.h>
#include <cs/Core/Flags.h>

namespace cs {

  enum class LineFlag : unsigned {
    None      = 0,
    SkipBlank = 0x01,
    Trim      = 0x02,
    All       = 0xFF
  };

} // namespace cs

CS_ENABLE_FLAGS(cs::LineFlag);

namespace cs {

  using LineFlags = Flags<LineFlag>;

  CS_UTIL_EXPORT std::string readInput(std::istream& stream,
                                       const std::size_t count = 128, const char delim = '\n');

  CS_UTIL_EXPORT std::list<std::string> readLines(const std::filesystem::path& path,
                                                  const LineFlags flags = LineFlag::None);

  CS_UTIL_EXPORT std::string readTextFile(const std::filesystem::path& path,
                                          bool *ok = nullptr);

} // namespace cs
