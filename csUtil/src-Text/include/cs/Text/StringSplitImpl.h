/****************************************************************************
** Copyright (c) 2024, Carsten Schmidt. All rights reserved.
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

#include <list>
#include <string>

#include <cs/Text/StringAlgorithm.h>

namespace cs {

  namespace impl_string {

    template<typename T> requires is_char_v<T>
    inline void extract(std::list<std::basic_string<T>>& result,
                        const T *first, const T *last,
                        const bool skipEmpty, const bool doTrim)
    {
      const std::size_t len = strlen(first, last);
      std::basic_string<T> part = len > 0
          ? std::basic_string<T>(first, len)
          : std::basic_string<T>();

      if( doTrim  &&  !part.empty() ) {
        ::cs::trim(part.data(), part.size());
        part.resize(strlen(part.data(), part.size()));
      }

      if( skipEmpty  &&  part.empty() ) {
        return;
      }

      result.push_back(std::move(part));
    }

    template<typename T> requires is_char_v<T>
    inline std::list<std::basic_string<T>> split(const T *first, const T *last,
                                                 const T *pat, const std::size_t maxpat,
                                                 const bool skipEmpty, const bool doTrim)
    {
      std::list<std::basic_string<T>> result;

      const T *from = first;
      for(const T *hit = nullptr;
          (hit = std::search(from, last, pat, pat + maxpat)) != last;
          from = hit + maxpat) {
        extract(result, from, hit, skipEmpty, doTrim);
      }
      extract(result, from, last, skipEmpty, doTrim);

      return result;
    }

    template<typename T> requires is_char_v<T>
    inline std::list<std::basic_string<T>> split(const T *first, const T *last,
                                                 const T& pat,
                                                 const bool skipEmpty, const bool doTrim)
    {
      std::list<std::basic_string<T>> result;

      const T *from = first;
      for(const T *hit = nullptr;
          (hit = std::find(from, last, pat)) != last;
          from = hit + 1) {
        extract(result, from, hit, skipEmpty, doTrim);
      }
      extract(result, from, last, skipEmpty, doTrim);

      return result;
    }

  } // namespace impl_string

} // namespace cs
