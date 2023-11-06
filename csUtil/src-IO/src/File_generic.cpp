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

#include "cs/Core/Container.h"
#include "cs/Core/TypeTraits.h"
#include "cs/IO/File.h"

namespace cs {

  Buffer File::readAll() const
  {
    constexpr offset_type MAX_OFFSET = maxab_v<offset_type,std::size_t>;
    constexpr offset_type        ONE = 1;

    const offset_type sizFile = size(); // NOTE: checks isOpen()

    const bool size_ok = ONE <= sizFile  &&  sizFile <= MAX_OFFSET;
    if( !size_ok ) {
      return Buffer();
    }

    // NOTE: Cast is guarded by 'size_ok'.
    const std::size_t numToRead = static_cast<std::size_t>(sizFile);

    Buffer buffer;
    if( !resize(&buffer, numToRead) ) {
      return Buffer();
    }

    if( read(buffer.data(), buffer.size()) != numToRead ) {
      return Buffer();
    }

    return buffer;
  }

} // namespace cs
