/****************************************************************************
** Copyright (c) 2022, Carsten Schmidt. All rights reserved.
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

#include <ostream>
#include <utility>

#include <cs/Core/Range.h>

namespace cs {

  namespace impl_print {

    template<typename CharT>
    requires IsCharacter<CharT>
    void print(std::basic_ostream<CharT> *stream, const CharT *first, const CharT *last)
    {
      if( !Pointer::isValidRange(first, last) ) {
        return;
      }
      stream->write(first, strlen(first, last));
    }

    template<typename CharT, typename T, typename ...Args>
    requires IsCharacter<CharT>
    void print(std::basic_ostream<CharT> *stream, const CharT *first, const CharT *last,
               T value, Args&&... args)
    {
      const CharT *hit = std::find(first, last, glyph<CharT>::pct);
      if( hit == last ) {
        print(stream, first, last);
        return;
      }

      print(stream, first, hit);
      *stream << value;
      print(stream, hit + 1, last, std::forward<Args>(args)...);
    }

  } // namespace impl_print

  ////// User Interface //////////////////////////////////////////////////////

  template<typename CharT, typename ...Args>
  requires IsCharacter<CharT>
  void print(std::basic_ostream<CharT> *stream, const CharT *fmt, Args&&... args)
  {
    impl_print::print(stream, fmt, fmt + strlen(fmt), std::forward<Args>(args)...);
  }

  template<typename CharT, typename ...Args>
  requires IsCharacter<CharT>
  void println(std::basic_ostream<CharT> *stream, const CharT *fmt, Args&&... args)
  {
    print(stream, fmt, std::forward<Args>(args)...);
    *stream << std::endl;
  }

} // namespace cs
