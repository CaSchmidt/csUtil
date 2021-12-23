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

#include "csUtil/csFileIO.h"

#include "csUtil/csFile.h"
#include "csUtil/csStringUtil.h"

CS_UTIL_EXPORT std::list<std::string> csReadLines(const std::u8string& filename, const bool trim)
{
  using StringList = std::list<std::string>;

  const std::string text = csReadTextFile(filename);
  if( text.empty() ) {
    return StringList();
  }

  return cs::split(text, '\n', true, trim);
}

CS_UTIL_EXPORT std::string csReadTextFile(const std::u8string& filename, bool *ok)
{
  using Buffer = std::vector<uint8_t>;
  using String = std::string;

  if( ok != nullptr ) {
    *ok = false;
  }

  csFile file;
  if( !file.open(filename) ) {
    return String();
  }
  const Buffer buffer = file.readAll();
  file.close();

  if( buffer.empty() ) {
    return String();
  }

  const String text(reinterpret_cast<const char*>(buffer.data()), buffer.size());

  if( ok != nullptr ) {
    *ok = true;
  }

  return text;
}
