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

#include "cs/System/FileSystem.h"

namespace cs {

  ////// Private /////////////////////////////////////////////////////////////

  namespace fs = std::filesystem;

  namespace impl_fs {

    inline void append(PathList& list, const fs::path& p, const PathListFlags flags)
    {
      if(        flags.testAny(PathListFlag::Directory)  &&  isDirectory(p) ) {
        list.push_back(p);

      } else if( flags.testAny(PathListFlag::File)  &&  isFile(p) ) {
        if(        flags.testAny(PathListFlag::SelectFilename) ) {
          list.push_back(p.filename());
        } else if( flags.testAny(PathListFlag::SelectStem) ) {
          list.push_back(p.stem());
        } else if( flags.testAny(PathListFlag::SelectExtension) ) {
          list.push_back(p.extension());
        } else {
          list.push_back(p);
        }

      }
    }

  } // namespace impl_fs

  ////// Public //////////////////////////////////////////////////////////////

  CS_UTIL_EXPORT PathList filter(const PathList& list, const PathListFlags flags)
  {
    PathList result;
    try {
      for(const fs::path& p : list) {
        impl_fs::append(result, p, flags);
      }
    } catch(...) {
      return PathList{};
    }

    return result;
  }

  CS_UTIL_EXPORT bool isDirectory(const fs::path& p)
  {
    std::error_code ec;
    return !p.empty()  &&  fs::is_directory(p, ec);
  }

  CS_UTIL_EXPORT bool isFile(const fs::path& p)
  {
    std::error_code ec;
    return !p.empty()  &&  fs::is_regular_file(p, ec);
  }

  CS_UTIL_EXPORT PathList list(const fs::path& p, const PathListFlags flags)
  {
    if( !isDirectory(p) ) {
      return PathList{};
    }

    PathList result;
    try {
      for(const fs::directory_entry& entry : makeDirIter(p)) {
        if( !flags.testAny(PathListFlag::AllTypes) ) {
          result.push_back(entry.path());
        } else {
          impl_fs::append(result, entry.path(), flags);
        }
      } // For Each Entry

      result.sort();
    } catch(...) {
      result.clear();
    }

    return result;
  }

  CS_UTIL_EXPORT fs::directory_iterator makeDirIter(const fs::path& p)
  {
    std::error_code ec;
    return isDirectory(p)
        ? fs::directory_iterator{p, fs::directory_options::skip_permission_denied, ec}
        : fs::directory_iterator{};
  }

  CS_UTIL_EXPORT std::filesystem::path reparent(const std::filesystem::path& parent,
                                                const std::filesystem::path& child)
  {
    return parent.parent_path() / child.filename();
  }

} // namespace cs
