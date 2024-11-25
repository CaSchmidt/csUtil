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

#include <chrono>
#include <filesystem>
#include <list>

#include <cs/Core/csutil_config.h>

#include <cs/Core/Flags.h>

namespace cs {

  enum class PathListFlag : unsigned int {
    None = 0,
    Directory = 1,
    File = 2,
    AllTypes = 0xFFFFFF,
    SelectFilename = 0x1000000,
    SelectStem = 0x2000000,
    SelectExtension = 0x4000000
  };

} // namespace cs

CS_ENABLE_FLAGS(cs::PathListFlag);

namespace cs {

  using PathListFlags = Flags<PathListFlag>;

  using PathList = std::list<std::filesystem::path>;

  using ConstPathListIter = PathList::const_iterator;

  using PathListIter = PathList::iterator;

  CS_UTIL_EXPORT PathList filter(const PathList& list, const PathListFlags flags);

  CS_UTIL_EXPORT bool isDirectory(const std::filesystem::path& p);

  CS_UTIL_EXPORT bool isFile(const std::filesystem::path& p);

  CS_UTIL_EXPORT std::chrono::system_clock::time_point lastWriteTime(const std::filesystem::path& p);

  CS_UTIL_EXPORT std::chrono::utc_clock::time_point lastWriteTimeUtc(const std::filesystem::path& p);

  CS_UTIL_EXPORT PathList list(const std::filesystem::path& p,
                               const PathListFlags flags = PathListFlag::None);

  CS_UTIL_EXPORT std::filesystem::directory_iterator makeDirIter(const std::filesystem::path& p);

  CS_UTIL_EXPORT std::filesystem::path reparent(const std::filesystem::path& parent,
                                                const std::filesystem::path& child);

} // namespace cs
