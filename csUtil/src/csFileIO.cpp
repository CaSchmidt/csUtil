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

#include <sstream>
#include <type_traits>

#include "csUtil/csFileIO.h"

#include "internal/csutil_internal.h"
#include "csUtil/csStringUtil.h"
#include "csUtil/csTextConverter.h"

////// Private ///////////////////////////////////////////////////////////////

namespace impl {

  template<typename T>
  inline T read(const std::u8string& filename, bool *ok)
  {
    static_assert(sizeof(typename T::value_type) == 1);

    T result;

    if( ok != nullptr ) {
      *ok = false;
    }

    try {
      std::fstream file = csOpenFile(filename, cs::READ_BINARY_FILE);
      if( !file.is_open() ) {
        return result;
      }

      file.seekg(0, std::ios_base::end);
      const auto size = file.tellg();
      file.seekg(0);

      result.resize(static_cast<typename T::size_type>(size));

      file.read(reinterpret_cast<char*>(result.data()), size);
    } catch(...) {
      result.clear();
      return result;
    }

    if( ok != nullptr ) {
      *ok = true;
    }

    return result;
  }

} // namespace impl

////// Public ////////////////////////////////////////////////////////////////

CS_UTIL_EXPORT std::fstream csOpenFile(const std::u8string& filename,
                                       const std::ios_base::openmode mode)
{
  std::fstream file;
#if defined(_MSC_VER) && defined(_WIN32)
  const std::u16string filename_utf16 = csUtf8ToUnicode(filename);
  file.open(cs::WSTR(filename_utf16.data()), mode);
#else
  file.open(cs::toString(filename), mode);
#endif
  return file;
}

CS_UTIL_EXPORT std::list<std::string> csReadLines(const std::u8string& filename, const bool trim)
{
  std::list<std::string> result;

  const std::string lines = csReadTextFile(filename);
  if( lines.empty() ) {
    return result;
  }

  std::istringstream input(lines);
  for(std::string line; std::getline(input, line, '\n'); ) {
    if( cs::isSpace(line.data(), line.size()) ) {
      continue;
    }
    if( trim ) {
      line = cs::trimmed(line);
    }
    result.push_back(std::move(line));
  }

  return result;
}

CS_UTIL_EXPORT std::string csReadTextFile(const std::u8string& filename, bool *ok)
{
  return impl::read<std::string>(filename, ok);
}

CS_UTIL_EXPORT std::vector<uint8_t> csReadBinaryFile(const std::u8string& filename, bool *ok)
{
  return impl::read<std::vector<uint8_t>>(filename, ok);
}

CS_UTIL_EXPORT bool csRead(std::istream& stream, void *data, const std::size_t size)
{
  using char_type = std::istream::char_type;
  static_assert(sizeof(char_type) == 1);
  try {
    stream.read(reinterpret_cast<char_type*>(data), static_cast<std::streamsize>(size));
  } catch(...) {
    return false;
  }
  return true;
}

CS_UTIL_EXPORT std::size_t csSize(std::istream& stream, bool *ok)
{
  using stream_type = std::remove_reference_t<decltype(stream)>;
  static_assert(sizeof(stream_type::char_type) == 1);
  if( ok != nullptr ) {
    *ok = false;
  }
  std::size_t size = 0;
  try {
    const stream_type::pos_type old = stream.tellg();
    stream.seekg(0, std::ios_base::end);
    size = static_cast<std::size_t>(stream.tellg());
    stream.seekg(old);
  } catch(...) {
    return 0;
  }
  if( ok != nullptr ) {
    *ok = true;
  }
  return size;
}

CS_UTIL_EXPORT std::size_t csSize(std::ostream& stream, bool *ok)
{
  using stream_type = std::remove_reference_t<decltype(stream)>;
  static_assert(sizeof(stream_type::char_type) == 1);
  if( ok != nullptr ) {
    *ok = false;
  }
  std::size_t size = 0;
  try {
    const stream_type::pos_type old = stream.tellp();
    stream.seekp(0, std::ios_base::end);
    size = static_cast<std::size_t>(stream.tellp());
    stream.seekp(old);
  } catch(...) {
    return 0;
  }
  if( ok != nullptr ) {
    *ok = true;
  }
  return size;
}

CS_UTIL_EXPORT bool csWrite(std::ostream& stream, const void *data, const std::size_t size)
{
  using char_type = std::ostream::char_type;
  static_assert(sizeof(char_type) == 1);
  try {
    stream.write(reinterpret_cast<const char_type*>(data), static_cast<std::streamsize>(size));
    stream.flush();
  } catch(...) {
    return false;
  }
  return true;
}
