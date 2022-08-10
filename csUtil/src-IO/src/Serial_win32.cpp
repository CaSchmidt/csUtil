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

#include "cs/IO/Serial.h"

#include "internal/Win32Handle.h"

namespace cs {

  constexpr DWORD QUEUE_SIZE = 8*1024;

  ////// Implementation //////////////////////////////////////////////////////

  class SerialImpl : public Win32Handle {
  public:
    SerialImpl() noexcept
      : Win32Handle()
    {
    }

    ~SerialImpl() noexcept
    {
    }
  };

  ////// Private /////////////////////////////////////////////////////////////

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

  ////// public //////////////////////////////////////////////////////////////

  Serial::Serial() noexcept
    : _impl{nullptr}
  {
  }

  Serial::~Serial() noexcept
  {
  }

  void Serial::close()
  {
    _impl.reset(nullptr);
  }

  bool Serial::isOpen() const
  {
    return _impl  &&  _impl->isOpen();
  }

  bool Serial::open(const std::filesystem::path& device, const int rate)
  {
    // (1) Close (possibly) open device //////////////////////////////////////

    close();

    // (2) Sanity check //////////////////////////////////////////////////////

    if( device.empty() ) {
      return false;
    }

    // (3) Create implementation /////////////////////////////////////////////

    try {
      _impl = std::make_unique<SerialImpl>();
    } catch(...) {
      return false;
    }

    // (4) Open device ///////////////////////////////////////////////////////

    const std::u16string device_utf16 = device.filename().generic_u16string();
    _impl->handle = CreateFileW(reinterpret_cast<LPCWSTR>(device_utf16.data()),
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

    // (5) Initialize device /////////////////////////////////////////////////

    if( SetupComm(_impl->handle, QUEUE_SIZE, QUEUE_SIZE) == 0 ) {
      close();
      return false;
    }

    DCB dcb;
    priv::setupDCB(&dcb, priv::toBaudRate(rate), 8, NOPARITY, ONESTOPBIT);
    if( SetCommState(_impl->handle, &dcb) == 0 ) {
      close();
      return false;
    }

    return isOpen();
  }

  IODevice::size_type Serial::read(void *buffer, const size_type length) const
  {
    if( !isOpen() ) {
      return 0;
    }

    if( !priv::handleError(_impl->handle, CE_OVERRUN | CE_RXOVER, PURGE_RXABORT | PURGE_RXCLEAR) ) {
      return 0;
    }

    return _impl->read(buffer, length);
  }

  IODevice::size_type Serial::write(const void *buffer, const size_type length) const
  {
    return isOpen()
        ? _impl->write(buffer, length)
        : 0;
  }

} // namespace cs
