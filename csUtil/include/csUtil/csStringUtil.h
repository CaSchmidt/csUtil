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

#ifndef CSSTRINGUTIL_H
#define CSSTRINGUTIL_H

#include <algorithm>
#include <string>

#include <csUtil/csCharUtil.h>

namespace cs {

  ////// Types ///////////////////////////////////////////////////////////////

  template<typename T>
  using if_string_t = typename std::enable_if<is_char<T>::value,std::basic_string<T>>::type;

  ////// Implementation //////////////////////////////////////////////////////

  template<typename T>
  inline std::size_t length(const T *s) noexcept
  {
    return s != nullptr
        ? std::char_traits<T>::length(s)
        : 0;
  }

  template<typename T>
  inline std::size_t lengthDiff(const T *first, const T *last) noexcept
  {
    return first != nullptr  &&  first < last
        ? static_cast<std::size_t>(std::find(first, last, glyph<T>::null) - first)
        : 0;
  }

  template<typename T>
  inline std::size_t lengthN(const T *s, const std::size_t maxCount) noexcept
  {
    return lengthDiff(s, s + maxCount);
  }

  template<typename T>
  inline if_char_bool<T> endsWith(const T *s1, const std::size_t _l1,
                                  const T *s2, const std::size_t _l2,
                                  const bool ignoreCase = false) noexcept
  {
    const std::size_t l1 = _l1 == MAX_SIZE_T
        ? length(s1)
        : _l1;
    const std::size_t l2 = _l2 == MAX_SIZE_T
        ? length(s2)
        : _l2;

    if( l1 < 1  ||  l2 < 1  ||  l1 < l2 ) {
      return false;
    }

    return ignoreCase
        ? std::equal(s1 + l1 - l2, s1 + l1, s2, [](const T& a, const T& b) -> bool { return toLower(a) == toLower(b); })
        : std::equal(s1 + l1 - l2, s1 + l1, s2);
  }

  template<typename T>
  inline if_char_bool<T> equals(const T *s1, const T *s2,
                                const std::size_t _count = MAX_SIZE_T,
                                const bool ignoreCase = false) noexcept
  {
    const std::size_t count = _count == MAX_SIZE_T
        ? std::min(length(s1), length(s2))
        : _count;

    if( count < 1 ) {
      return false;
    }

    return ignoreCase
        ? std::equal(s1, s1 + count, s2, [](const T& a, const T& b) -> bool { return toLower(a) == toLower(b); })
        : std::equal(s1, s1 + count, s2);
  }

  template<typename T>
  inline if_char_bool<T> equals(const T *s1, const std::size_t len1,
                                const T *s2, const std::size_t len2,
                                const bool ignoreCase = false)
  {
    const std::size_t l1 = len1 == MAX_SIZE_T
        ? length(s1)
        : len1;
    const std::size_t l2 = len2 == MAX_SIZE_T
        ? length(s2)
        : len2;

    if( l1 < 1  ||  l2 < 1  ||  l1 != l2 ) {
      return false;
    }

    return ignoreCase
        ? std::equal(s1, s1 + l1, s2, [](const T& a, const T& b) -> bool { return toLower(a) == toLower(b); })
        : std::equal(s1, s1 + l1, s2);
  }

  template<typename T>
  inline if_char_bool<T> isSpace(const T *s, const std::size_t len = MAX_SIZE_T)
  {
    const std::size_t l = len == MAX_SIZE_T
        ? length(s)
        : len;
    const auto numSpace = std::count_if(s, s + l,
                                        [](const T& ch) -> bool { return isSpace(ch); });
    return static_cast<std::size_t>(numSpace) == l;
  }

  template<typename T>
  inline void replaceAll(std::basic_string<T>& s,
                         const T& needle,
                         const T *text, const std::size_t lenText = MAX_SIZE_T)
  {
    const std::size_t lt = lenText == MAX_SIZE_T
        ? length(text)
        : lenText;

    if( s.size() < 1  ||  lt < 1 ) {
      return;
    }

    for(std::size_t pos = 0; (pos = s.find(needle, pos)) != s.npos; pos += lt) {
      s.replace(pos, 1, text, lt);
    }
  }

  template<typename T>
  inline void replaceAll(std::basic_string<T>& s,
                         const T *needle, const std::size_t lenNeedle,
                         const T *text, const std::size_t lenText = MAX_SIZE_T)
  {
    const std::size_t ln = lenNeedle == MAX_SIZE_T
        ? length(needle)
        : lenNeedle;
    const std::size_t lt = lenText == MAX_SIZE_T
        ? length(text)
        : lenText;

    if( s.size() < 1  ||  ln < 1  ||  lt < 1  ||  s.size() < ln ) {
      return;
    }

    for(std::size_t pos = 0; (pos = s.find(needle, pos, ln)) != s.npos; pos += lt) {
      s.replace(pos, ln, text, lt);
    }
  }

  template<typename T>
  inline if_string_t<T> simplified(std::basic_string<T> s) noexcept
  {
    // (1) remove duplicate whitespace
    auto last = std::unique(s.begin(), s.end(),
                            [](const T& a, const T& b) -> bool { return isSpace(a)  &&  isSpace(b); });
    s.erase(last, s.end());
    // (2) replace single whitespace characters with space
    std::replace_if(s.begin(), s.end(),
                    [](const T& ch) -> bool { return isSpace(ch); },
                    glyph<T>::space);
    // (3) return trimmed result
    return trimmed(s);
  }

  template<typename T>
  inline if_char_bool<T> startsWith(const T *s1, const std::size_t _l1,
                                    const T *s2, const std::size_t _l2,
                                    const bool ignoreCase = false) noexcept
  {
    const std::size_t l1 = _l1 == MAX_SIZE_T
        ? length(s1)
        : _l1;
    const std::size_t l2 = _l2 == MAX_SIZE_T
        ? length(s2)
        : _l2;

    if( l1 < 1  ||  l2 < 1  ||  l1 < l2 ) {
      return false;
    }

    return ignoreCase
        ? std::equal(s1, s1 + l2, s2, [](const T& a, const T& b) -> bool { return toLower(a) == toLower(b); })
        : std::equal(s1, s1 + l2, s2);
  }

  template<typename T>
  inline void toLower(T *s, const std::size_t len = cs::MAX_SIZE_T)
  {
    const std::size_t l = len == cs::MAX_SIZE_T
        ? length(s)
        : len;

    if( l < 1 ) {
      return;
    }

    std::for_each(s, s + l, [](T& c) -> void { c = cs::toLower(c); });
  }

  template<typename T>
  inline void toUpper(T *s, const std::size_t len = cs::MAX_SIZE_T)
  {
    const std::size_t l = len == cs::MAX_SIZE_T
        ? length(s)
        : len;

    if( l < 1 ) {
      return;
    }

    std::for_each(s, s + l, [](T& c) -> void { c = cs::toUpper(c); });
  }

  template<typename T>
  inline if_string_t<T> trimmed(std::basic_string<T> s) noexcept
  {
    // trim left
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(),
                         [](const T& ch) -> bool { return !isSpace(ch); }));
    // trim right
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](const T& ch) -> bool { return !isSpace(ch); }).base(),
            s.end());
    return s;
  }

} // namespace cs

#endif // CSSTRINGUTIL_H
