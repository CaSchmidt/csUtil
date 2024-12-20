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

#include <string>

#include <cs/Core/Buffer.h>
#include <cs/IO/AbstractIODevice.h>

namespace cs {

  class CS_UTIL_EXPORT BufferedReader {
  public:
    using byte_type = Buffer::value_type;
    using size_type = Buffer::size_type;

    BufferedReader(const size_type sizeBuffer = 128*1024) noexcept;
    ~BufferedReader() noexcept;

    bool isEmpty() const;

    void reset();

    std::string getLine(const AbstractIODevice& dev, const char sep = '\n');

  private:
    static constexpr size_type ONE = 1;

    byte_type *beginData();
    byte_type *endData();

    const byte_type *cbeginData() const;
    const byte_type *cendData() const;

    template<typename T>
    const T *cbeginDataAs() const
    {
      return reinterpret_cast<const T*>(cbeginData());
    }

    void fillBuffer(const AbstractIODevice& dev);
    void syncBuffer(const AbstractIODevice& dev);

    size_type scanData(const byte_type sep) const;

    Buffer _buffer{};
    size_type _idxData{0};
    size_type _numData{0};
  };

} // namespace cs
