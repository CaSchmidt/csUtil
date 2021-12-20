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

#include "csUtil/csSerial.h"

#include "csUtil/csTextConverter.h"
#include "internal/Win32Handle.h"

////// Implementation ////////////////////////////////////////////////////////

class csSerialImpl : public Win32Handle {
public:
  csSerialImpl() noexcept
    : Win32Handle()
  {
  }

  ~csSerialImpl() noexcept
  {
  }
};

////// Private ///////////////////////////////////////////////////////////////

namespace priv {

  bool handleError(HANDLE handle,
                   const DWORD dwErrorMask, const DWORD dwPurgeMask)
  {
    DWORD dwErrors = 0;
    if( ClearCommError(handle, &dwErrors, NULL) == 0 ) {
      return false;
    }

    if( (dwErrors & dwErrorMask) != 0  &&  PurgeComm(handle, dwPurgeMask) == 0 ) {
      return false;
    }

    return true;
  }

  void setupDCB(DCB *dcb, const DWORD dwBaudRate,
                const BYTE bByteSize, const BYTE bParity, const BYTE bStopBits)
  {
    SecureZeroMemory(dcb, sizeof(DCB));
    dcb->DCBlength = sizeof(DCB);

    dcb->fBinary = 1;

    dcb->BaudRate = dwBaudRate;

    dcb->ByteSize = bByteSize;

    if( bParity != NOPARITY ) {
      dcb->fParity = 1;
      dcb->Parity = bParity;
    } else {
      dcb->fParity = 0;
    }

    dcb->StopBits = bStopBits;
  }

  DWORD toBaudRate(const int rate)
  {
    if(        rate == 110 ) {
      return CBR_110;
    } else if( rate == 300 ) {
      return CBR_300;
    } else if( rate == 600 ) {
      return CBR_600;
    } else if( rate == 1200 ) {
      return CBR_1200;
    } else if( rate == 2400 ) {
      return CBR_2400;
    } else if( rate == 4800 ) {
      return CBR_4800;
    } else if( rate == 9600 ) {
      return CBR_9600;
    } else if( rate == 14400 ) {
      return CBR_14400;
    } else if( rate == 19200 ) {
      return CBR_19200;
    } else if( rate == 38400 ) {
      return CBR_38400;
    } else if( rate == 57600 ) {
      return CBR_57600;
    } else if( rate == 115200 ) {
      return CBR_115200;
    } else if( rate == 128000 ) {
      return CBR_128000;
    } else if( rate == 256000 ) {
      return CBR_256000;
    }
    return 0;
  }

} // namespace priv

////// public ////////////////////////////////////////////////////////////////

csSerial::csSerial() noexcept
  : _impl{nullptr}
{
}

csSerial::~csSerial() noexcept
{
  csSerial::close();
}

void csSerial::close()
{
  _impl.reset(nullptr);
}

bool csSerial::isOpen() const
{
  return _impl  &&  _impl->isOpen();
}

bool csSerial::open(const std::u8string& device, const int rate)
{
  // (1) Close (possibly) open device ////////////////////////////////////////

  close();

  // (2) Create implementation ///////////////////////////////////////////////

  try {
    _impl = std::make_unique<csSerialImpl>();
  } catch (...) {
    return false;
  }

  // (3) Open device /////////////////////////////////////////////////////////

  const std::u16string filename_utf16 = csUtf8ToUnicode(device);
  _impl->handle = CreateFileW(reinterpret_cast<LPCWSTR>(filename_utf16.data()),
                              GENERIC_READ | GENERIC_WRITE,
                              0,
                              NULL,
                              OPEN_EXISTING,
                              0,
                              NULL);
  if( !isOpen() ) {
    close();
    return false;
  }

  // (4) Initialize device ///////////////////////////////////////////////////

  DCB dcb;
  priv::setupDCB(&dcb, priv::toBaudRate(rate), 8, NOPARITY, ONESTOPBIT);
  if( SetCommState(_impl->handle, &dcb) == 0 ) {
    close();
    return false;
  }

  return isOpen();
}

csIODevice::size_type csSerial::read(void *buffer, const size_type length) const
{
  if( !isOpen() ) {
    return 0;
  }

  if( !priv::handleError(_impl->handle, CE_OVERRUN | CE_RXOVER, PURGE_RXABORT | PURGE_RXCLEAR) ) {
    return 0;
  }

  return _impl->read(buffer, length);
}

csIODevice::size_type csSerial::write(const void *buffer, const size_type length) const
{
  return isOpen()
      ? _impl->write(buffer, length)
      : 0;
}
