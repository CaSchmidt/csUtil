/****************************************************************************
** Copyright (c) 2021, Carsten Schmidt. All rights reserved.
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

#include <limits>

#define NOMINMAX
#include <Windows.h>

#include "csUtil/csFile.h"

////// Implementation ////////////////////////////////////////////////////////

class csFileImpl {
public:
  csFileImpl() noexcept
    : file{INVALID_HANDLE_VALUE}
    , filename()
  {
  }

  ~csFileImpl() noexcept
  {
    if( isOpen() ) {
      CloseHandle(file);
    }
    file = INVALID_HANDLE_VALUE;
  }

  bool isOpen() const
  {
    return file != INVALID_HANDLE_VALUE;
  }

  bool seek(const csFile::filepos_t pos) const
  {
    LARGE_INTEGER li;
    li.QuadPart = pos;

    return isOpen()  &&  SetFilePointerEx(file, li, NULL, FILE_BEGIN) != 0;
  }

  bool seekToEnd() const
  {
    LARGE_INTEGER li;
    li.QuadPart = 0;

    return isOpen()  &&  SetFilePointerEx(file, li, NULL, FILE_END) != 0;
  }

  bool truncate() const
  {
    return isOpen()  &&  seek(0)  &&  SetEndOfFile(file) != 0;
  }

  HANDLE file{INVALID_HANDLE_VALUE};
  std::u16string filename;
};

////// Private ///////////////////////////////////////////////////////////////

namespace priv {

  inline bool isRWLength(const csFile::filesize_t length)
  {
    constexpr csFile::filesize_t MAX_DWORD = std::numeric_limits<DWORD>::max();
    constexpr csFile::filesize_t       ONE = 1;

    return ONE <= length  &&  length <= MAX_DWORD;
  }

} // namespace priv

////// public ////////////////////////////////////////////////////////////////

csFile::csFile() noexcept
  : _impl{nullptr}
{
}

csFile::~csFile() noexcept
{
  close();
}

void csFile::close()
{
  _impl.reset(nullptr);
}

bool csFile::isOpen() const
{
  return _impl  &&  _impl->isOpen();
}

bool csFile::open(const std::u16string& filename, const OpenFlags flags)
{
  // (1) Close (possibly) open file //////////////////////////////////////////

  close();

  // (2) Sanity check ////////////////////////////////////////////////////////

  if( !flags.testFlag(cs::FileOpenFlag::ReadWrite) ) {
    return false;
  }

  // (3) Allocate file handle ////////////////////////////////////////////////

  try {
    _impl = std::make_unique<csFileImpl>();
  } catch(...) {
    return false;
  }

  // (4) Open file ///////////////////////////////////////////////////////////

  DWORD dwDesiredAccess = 0;
  if( flags.testFlag(cs::FileOpenFlag::Read) ) {
    dwDesiredAccess |= GENERIC_READ;
  }
  if( flags.testFlag(cs::FileOpenFlag::Write) ) {
    dwDesiredAccess |= GENERIC_WRITE;
  }

  DWORD dwCreationDisposition = 0;
  if( flags.testFlag(cs::FileOpenFlag::Write) ) {
    dwCreationDisposition = flags.testFlag(cs::FileOpenFlag::Append)
        ? OPEN_ALWAYS
        : CREATE_ALWAYS;
  } else {
    dwCreationDisposition = OPEN_EXISTING;
  }

  _impl->file = CreateFileW(reinterpret_cast<LPCWSTR>(filename.data()),
                            dwDesiredAccess,
                            0,
                            NULL,
                            dwCreationDisposition,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

  if( !isOpen() ) {
    _impl->filename = filename;
  }

  // (5) Handling of remaining flags /////////////////////////////////////////

  if( flags.testMask(cs::FileOpenFlag::Write | cs::FileOpenFlag::Append) ) {
    _impl->seekToEnd();
  }
  if( flags.testMask(cs::FileOpenFlag::Write | cs::FileOpenFlag::Truncate) ) {
    _impl->truncate();
  }

  return isOpen();
}

std::u16string csFile::filename() const
{
  if( !isOpen() ) {
    return std::u16string();
  }
  return _impl->filename;
}

bool csFile::seek(const filepos_t pos) const
{
  return isOpen()  &&  _impl->seek(pos);
}

csFile::filepos_t csFile::tell() const
{
  LARGE_INTEGER in;
  in.QuadPart = 0;
  LARGE_INTEGER out;
  out.QuadPart = 0;

  if( !isOpen()  ||  SetFilePointerEx(_impl->file, in, &out, FILE_CURRENT) == 0 ) {
    return 0;
  }

  return out.QuadPart;
}

csFile::filesize_t csFile::size() const
{
  LARGE_INTEGER li;
  li.QuadPart = 0;

  if( !isOpen()  ||  GetFileSizeEx(_impl->file, &li) == 0 ) {
    return 0;
  }

  return static_cast<filesize_t>(li.QuadPart);
}

csFile::filesize_t csFile::read(void *buffer, const filesize_t length) const
{
  const DWORD numToRead = static_cast<DWORD>(length);
  DWORD numRead = 0;
  if( !isOpen()  ||  !priv::isRWLength(length)  ||
      ReadFile(_impl->file, buffer, numToRead, &numRead, NULL) == 0 ) {
    return 0;
  }
  return numRead;
}

csFile::filesize_t csFile::write(const void *buffer, const filesize_t length) const
{
  const DWORD numToWrite = static_cast<DWORD>(length);
  DWORD numWritten = 0;
  if( !isOpen()  ||  !priv::isRWLength(length)  ||
      WriteFile(_impl->file, buffer, numToWrite, &numWritten, NULL) == 0 ) {
    return 0;
  }
  return numWritten;
}
