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

#include <algorithm>
#include <iterator>

#include <cs/Core/Container.h>

#include "cs/IO/BufferedReader.h"

namespace cs {

  ////// public //////////////////////////////////////////////////////////////

  static_assert( std::is_unsigned_v<BufferedReader::size_type> );

  BufferedReader::BufferedReader(const size_type sizeBuffer) noexcept
    : _buffer{}
  {
    reset();
    resize(_buffer, sizeBuffer);
  }

  BufferedReader::~BufferedReader() noexcept
  {
  }

  bool BufferedReader::isEmpty() const
  {
    return _buffer.empty();
  }

  void BufferedReader::reset()
  {
    _idxData = _numData = 0;
  }

  std::string BufferedReader::getLine(const AbstractIODevice& dev, const char sep)
  {
    static_assert( sizeof(byte_type) == sizeof(char) );

    // (1) Fill buffer if it is not (yet) filled OR fully consumed. //////////

    if( _numData < ONE  ||  _idxData == _numData ) {
      fillBuffer(dev);
    }

    if( _numData < ONE ) {
      return std::string();
    }

    // (2) Scan Data Buffer //////////////////////////////////////////////////

    size_type hit = scanData(sep);
    if( hit == _numData ) {
      syncBuffer(dev);
      hit = scanData(sep);
    }

    // (3) Create Data View //////////////////////////////////////////////////

    std::string result;
    if( hit - _idxData >= ONE ) {
      try {
        result.assign(cbeginDataAs<char>(), hit - _idxData);
      } catch(...) {
        return std::string();
      }
    }

    // (4) Advance Index /////////////////////////////////////////////////////

    _idxData = std::min(hit + ONE, _numData);

    return result;
  }

  ////// private /////////////////////////////////////////////////////////////

  BufferedReader::byte_type *BufferedReader::beginData()
  {
    return _buffer.data() + _idxData;
  }

  BufferedReader::byte_type *BufferedReader::endData()
  {
    return _buffer.data() + _numData;
  }

  const BufferedReader::byte_type *BufferedReader::cbeginData() const
  {
    return _buffer.data() + _idxData;
  }

  const BufferedReader::byte_type *BufferedReader::cendData() const
  {
    return _buffer.data() + _numData;
  }

  void BufferedReader::fillBuffer(const AbstractIODevice& dev)
  {
    _numData = dev.read(_buffer.data(), _buffer.size());
    _idxData = 0;
  }

  void BufferedReader::syncBuffer(const AbstractIODevice& dev)
  {
    // (1) Copy Unconsumed Data //////////////////////////////////////////////

    if( _idxData >= ONE  &&  _idxData <= _numData ) {
      std::copy(cbeginData(), cendData(), _buffer.data());
      _numData -= _idxData;
      _idxData  = 0;
    }

    // (2) Fill Buffer ///////////////////////////////////////////////////////

    if( _numData < _buffer.size() ) {
      _numData += dev.read(endData(), _buffer.size() - _numData);
    }
  }

  BufferedReader::size_type BufferedReader::scanData(const byte_type sep) const
  {
    return std::distance(_buffer.data(),
                         std::find(cbeginData(), cendData(), sep));
  }

} // namespace cs
