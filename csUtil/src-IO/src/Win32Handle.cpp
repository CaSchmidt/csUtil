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

#include "cs/Core/TypeTraits.h"

#include "internal/Win32Handle.h"

namespace cs {

  ////// Private /////////////////////////////////////////////////////////////

  namespace impl_win32 {

    inline bool checkRwSize(const std::size_t size)
    {
      constexpr std::size_t MAX_RW_SIZE = maxab_v<std::size_t,DWORD>;
      constexpr std::size_t         ONE = 1;

      return ONE <= size  &&  size <= MAX_RW_SIZE;
    }

  } // namespace impl_win32

  ////// public //////////////////////////////////////////////////////////////

  Win32Handle::Win32Handle() noexcept
    : handle{INVALID_HANDLE_VALUE}
    , path{}
  {
  }

  Win32Handle::~Win32Handle() noexcept
  {
    if( isOpen() ) {
      CloseHandle(handle);
    }
    handle = INVALID_HANDLE_VALUE;
    path.clear();
  }

  bool Win32Handle::isOpen() const
  {
    return handle != INVALID_HANDLE_VALUE;
  }

  std::size_t Win32Handle::read(void *data, const std::size_t sizData) const
  {
    if( !impl_win32::checkRwSize(sizData) ) {
      return 0;
    }

    const DWORD numToRead = static_cast<DWORD>(sizData);
    DWORD numRead = 0;
    return isOpen()  &&  ReadFile(handle, data, numToRead, &numRead, NULL) != 0
        ? static_cast<std::size_t>(numRead)
        : 0;
  }

  std::size_t Win32Handle::write(const void *data, const std::size_t sizData) const
  {
    if( !impl_win32::checkRwSize(sizData) ) {
      return 0;
    }

    const DWORD numToWrite = static_cast<DWORD>(sizData);
    DWORD numWritten = 0;
    return isOpen()  &&  WriteFile(handle, data, numToWrite, &numWritten, NULL) != 0
        ? static_cast<std::size_t>(numWritten)
        : 0;
  }

} // namespace cs
