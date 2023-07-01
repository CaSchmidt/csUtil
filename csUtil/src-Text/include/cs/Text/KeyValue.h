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

#include <cs/Core/Iterator.h>
#include <cs/Text/StringUtil.h>

namespace cs {

  template<typename T> requires IsCharacter<T>
  using KeyValue = std::pair<String<T>,String<T>>;

  template<typename T>
  using KeyValues = std::list<KeyValue<T>>;

  namespace impl_keyvalue {

    template<typename T> requires IsCharacter<T>
    inline KeyValues<T> makeKeyValues(const T *first, const T *last)
    {
      constexpr T SEP_PAIR  = glyph<T>::comma;
      constexpr T SEP_VALUE = glyph<T>::colon;

      KeyValues<T> result;
      for(const T *begPair = first; begPair != last; ) {
        const T *endPair = std::find(begPair, last, SEP_PAIR);

        if( begPair < endPair ) {
          const T *posValue = std::find(begPair, endPair, SEP_VALUE);

          const String<T> key{begPair, posValue};
          const String<T> value{succ(posValue, endPair), endPair};

          if( !key.empty() ) {
            result.emplace_back(key, value);
          }
        }

        begPair = succ(endPair, last);
      }

      return result;
    }

  } // namespace impl_keyvalue

  template<typename T> requires IsCharacter<T>
  inline KeyValues<T> makeKeyValues(const T *first, const T *last)
  {
    return Pointer::isValidRange(first, last)
        ? impl_keyvalue::makeKeyValues(first, last)
        : KeyValues<T>{};
  }

  template<typename T> requires IsCharacter<T>
  inline KeyValues<T> makeKeyValues(const T *str, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t max = strlen(str, len);
    return max > 0
        ? impl_keyvalue::makeKeyValues(str, str + max)
        : KeyValues<T>{};
  }

  template<typename T> requires IsCharacter<T>
  inline KeyValues<T> makeKeyValues(const String<T>& str)
  {
    return makeKeyValues(str.data(), str.size());
  }

  template<typename T> requires IsCharacter<T>
  inline KeyValues<T> makeKeyValues(const StringView<T>& view)
  {
    return makeKeyValues(view.data(), view.size());
  }

} // namespace cs
