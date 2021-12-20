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

#include "csUtil/csFile.h"

#include "csUtil/csTextConverter.h"
#include "internal/Win32Handle.h"

////// Implementation ////////////////////////////////////////////////////////

class csFileImpl : public Win32Handle {
public:
  csFileImpl() noexcept
    : Win32Handle()
  {
  }

  ~csFileImpl() noexcept
  {
  }

  bool seek(const csIODevice::pos_type pos) const
  {
    LARGE_INTEGER li;
    li.QuadPart = pos;

    return isOpen()  &&  SetFilePointerEx(handle, li, NULL, FILE_BEGIN) != 0;
  }

  bool seekToEnd() const
  {
    LARGE_INTEGER li;
    li.QuadPart = 0;

    return isOpen()  &&  SetFilePointerEx(handle, li, NULL, FILE_END) != 0;
  }

  bool truncate() const
  {
    return isOpen()  &&  seek(0)  &&  SetEndOfFile(handle) != 0;
  }
};

////// public ////////////////////////////////////////////////////////////////

csFile::csFile() noexcept
  : _impl{nullptr}
{
}

csFile::~csFile() noexcept
{
  csFile::close();
}

void csFile::close()
{
  _impl.reset(nullptr);
}

bool csFile::isOpen() const
{
  return _impl  &&  _impl->isOpen();
}

bool csFile::open(const std::u8string& filename, const OpenFlags flags)
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

  const std::u16string filename_utf16 = csUtf8ToUnicode(filename);
  _impl->handle = CreateFileW(reinterpret_cast<LPCWSTR>(filename_utf16.data()),
                              dwDesiredAccess,
                              0,
                              NULL,
                              dwCreationDisposition,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);

  // (5) Handling of remaining flags /////////////////////////////////////////

  if( isOpen() ) {
    _impl->name = filename;

    if( flags.testMask(cs::FileOpenFlag::Write | cs::FileOpenFlag::Append) ) {
      _impl->seekToEnd();
    }
    if( flags.testMask(cs::FileOpenFlag::Write | cs::FileOpenFlag::Truncate) ) {
      _impl->truncate();
    }
  } else {
    close();
  }

  return isOpen();
}

std::u8string csFile::filename() const
{
  if( !isOpen() ) {
    return std::u8string();
  }
  return _impl->name;
}

bool csFile::seek(const pos_type pos) const
{
  return isOpen()  &&  _impl->seek(pos);
}

csIODevice::pos_type csFile::tell() const
{
  LARGE_INTEGER in;
  in.QuadPart = 0;
  LARGE_INTEGER out;
  out.QuadPart = 0;

  if( !isOpen()  ||  SetFilePointerEx(_impl->handle, in, &out, FILE_CURRENT) == 0 ) {
    return 0;
  }

  return out.QuadPart;
}

csIODevice::size_type csFile::size() const
{
  LARGE_INTEGER li;
  li.QuadPart = 0;

  if( !isOpen()  ||  GetFileSizeEx(_impl->handle, &li) == 0 ) {
    return 0;
  }

  return static_cast<size_type>(li.QuadPart);
}

csIODevice::size_type csFile::read(void *buffer, const size_type length) const
{
  return isOpen()
      ? _impl->read(buffer, length)
      : 0;
}

csIODevice::size_type csFile::write(const void *buffer, const size_type length) const
{
  return isOpen()
      ? _impl->write(buffer, length)
      : 0;
}
