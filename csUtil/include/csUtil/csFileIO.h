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

#ifndef CSFILEIO_H
#define CSFILEIO_H

#include <cstdint>

#include <fstream>
#include <list>
#include <string>
#include <vector>

#include <csUtil/csutil_config.h>

namespace cs {

  constexpr std::ios_base::openmode CREATE_BINARY_FILE =
      std::ios_base::out | std::ios_base::binary | std::ios_base::trunc;

  constexpr std::ios_base::openmode READ_BINARY_FILE =
      std::ios_base::in | std::ios_base::binary;

} // namespace cs

CS_UTIL_EXPORT std::fstream csOpenFile(const std::string& filename_utf8,
                                       const std::ios_base::openmode mode);

CS_UTIL_EXPORT std::list<std::string> csReadLines(const std::string& filename_utf8, const bool trim = false);

CS_UTIL_EXPORT std::string csReadTextFile(const std::string& filename_utf8, bool *ok = nullptr);

CS_UTIL_EXPORT std::vector<uint8_t> csReadBinaryFile(const std::string& filename_utf8, bool *ok = nullptr);

CS_UTIL_EXPORT bool csRead(std::istream& stream, void *data, const std::streamsize size);

CS_UTIL_EXPORT bool csWrite(std::ostream& stream, const void *data, const std::streamsize size);

#endif // CSFILEIO_H
