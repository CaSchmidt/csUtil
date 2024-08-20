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

#include <iostream>
#include <syncstream>

#include <cs/Text/Print.h>

namespace cs {

  // Print to narrow character stream ////////////////////////////////////////

  template<typename ...Args>
  void print(std::ostream& stream, const std::string_view& fmt, Args&&... args)
  {
    impl_print::print<char,Args...>(stream, fmt, std::forward<Args>(args)...);
  }

  template<typename ...Args>
  void println(std::ostream& stream, const std::string_view& fmt, Args&&... args)
  {
    impl_print::println<char,Args...>(stream, fmt, std::forward<Args>(args)...);
  }

  // Print to synchronized standard streams //////////////////////////////////

  template<typename ...Args>
  void print(const std::string_view& fmt, Args&&... args)
  {
    std::osyncstream syncstrm(std::cout);
    impl_print::print<char,Args...>(syncstrm, fmt, std::forward<Args>(args)...);
  }

  template<typename ...Args>
  void println(const std::string_view& fmt, Args&&... args)
  {
    std::osyncstream syncstrm(std::cout);
    impl_print::println<char,Args...>(syncstrm, fmt, std::forward<Args>(args)...);
  }

  template<typename ...Args>
  void printerr(const std::string_view& fmt, Args&&... args)
  {
    std::osyncstream syncstrm(std::cerr);
    impl_print::print<char,Args...>(syncstrm, fmt, std::forward<Args>(args)...);
  }

  template<typename ...Args>
  void printerrln(const std::string_view& fmt, Args&&... args)
  {
    std::osyncstream syncstrm(std::cerr);
    impl_print::println<char,Args...>(syncstrm, fmt, std::forward<Args>(args)...);
  }

  template<typename ...Args>
  void printlog(const std::string_view& fmt, Args&&... args)
  {
    std::osyncstream syncstrm(std::clog);
    impl_print::print<char,Args...>(syncstrm, fmt, std::forward<Args>(args)...);
  }

  template<typename ...Args>
  void printlogln(const std::string_view& fmt, Args&&... args)
  {
    std::osyncstream syncstrm(std::clog);
    impl_print::println<char,Args...>(syncstrm, fmt, std::forward<Args>(args)...);
  }

  // Print to narrow character string ////////////////////////////////////////

  template<typename ...Args>
  std::string sprint(const std::string_view& fmt, Args&&... args)
  {
    return impl_print::sprint<char,Args...>(fmt, std::forward<Args>(args)...);
  }

} // namespace cs
