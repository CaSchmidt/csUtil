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

#ifndef WIN32HANDLE_H
#define WIN32HANDLE_H

#include <string>

#define NOMINMAX
#include <Windows.h>

class Win32Handle {
public:
  Win32Handle() noexcept;
  ~Win32Handle() noexcept;

  bool isOpen() const;

  template<typename CS_SIZE_T>
  inline static bool isRWLength(const CS_SIZE_T length)
  {
    static_assert(std::is_integral_v<CS_SIZE_T>  &&  sizeof(CS_SIZE_T) > sizeof(DWORD));

    constexpr CS_SIZE_T MAX_DWORD = std::numeric_limits<DWORD>::max();
    constexpr CS_SIZE_T       ONE = 1;

    return ONE <= length  &&  length <= MAX_DWORD;
  }

  template<typename CS_SIZE_T>
  inline CS_SIZE_T read(void *buffer, const CS_SIZE_T length) const
  {
    const DWORD numToRead = static_cast<DWORD>(length);
    DWORD numRead = 0;
    if( !isOpen()  ||  !isRWLength(length)  ||
        ReadFile(handle, buffer, numToRead, &numRead, NULL) == 0 ) {
      return 0;
    }
    return numRead;
  }

  template<typename CS_SIZE_T>
  inline CS_SIZE_T write(const void *buffer, const CS_SIZE_T length) const
  {
    const DWORD numToWrite = static_cast<DWORD>(length);
    DWORD numWritten = 0;
    if( !isOpen()  ||  !isRWLength(length)  ||
        WriteFile(handle, buffer, numToWrite, &numWritten, NULL) == 0 ) {
      return 0;
    }
    return numWritten;
  }

  HANDLE handle{INVALID_HANDLE_VALUE};
  std::u8string name;
};

#endif // WIN32HANDLE_H