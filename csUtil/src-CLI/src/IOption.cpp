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

#include <algorithm>
#include <print>

#include "cs/CLI/IOption.h"

#include "cs/Text/StringUtil.h"

namespace cs {

  ////// public //////////////////////////////////////////////////////////////

  IOption::~IOption() noexcept
  {
  }

  std::string IOption::help() const
  {
    return _help;
  }

  void IOption::setHelp(const std::string& help)
  {
    _help = help;
  }

  bool IOption::isLongFormat() const
  {
    return _isLongFormat;
  }

  void IOption::setLongFormat(const bool on)
  {
    _isLongFormat = on;
    initializePrefix();
  }

  bool IOption::isRequired() const
  {
    return _isRequired;
  }

  void IOption::setRequired(const bool on)
  {
    _isRequired = on;
  }

  const char *IOption::name() const
  {
    return _name.data();
  }

  bool IOption::isOption(const char *arg) const
  {
    return startsWith(arg, _prefix.data());
  }

  bool IOption::parse(const char *arg)
  {
    return isOption(arg)  &&  impl_parse(arg + _prefix.size());
  }

  bool IOption::isValid() const
  {
    return impl_isValid();
  }

  void IOption::printUsage(std::ostream& output) const
  {
    if( isRequired() ) {
      std::print(output, "    ");
    } else {
      std::print(output, "  ? ");
    }

    std::print(output, "{}", _prefix);

    if( isValueOption() ) {
      std::print(output, "<{}>", impl_defaultValue());
    }

    if( !_help.empty() ) {
      const std::list<std::string> lines = split(_help, '\n');
      for(const std::string& line : lines) {
        std::println(output, "");
        std::print(output, "        {}", line);
      }
    }

    std::println(output, "");
  }

  bool IOption::isValidName(const char *s)
  {
    const auto cnt = std::count_if(s, s + strlen(s),
                                   [](const char c) -> bool { return isSpace(c); });
    return static_cast<std::size_t>(cnt) < 1;
  }

  ////// protected ///////////////////////////////////////////////////////////

  IOption::IOption(const std::string& name) noexcept
    : _name(name)
  {
  }

  ////// private /////////////////////////////////////////////////////////////

  void IOption::initializePrefix()
  {
    try {
      _prefix.clear();
      _prefix.reserve(32);

      _prefix = '-';
      if( isLongFormat() ) {
        _prefix += '-';
      }

      _prefix += _name;

      if( isValueOption() ) {
        _prefix += '=';
      }
    } catch(...) {
      _prefix.clear();
    }
  }

  bool IOption::isValueOption() const
  {
    return impl_defaultValue() != nullptr;
  }

} // namespace cs
