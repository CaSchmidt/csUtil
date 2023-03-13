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

#include <memory>
#include <ostream>
#include <string>
#include <utility>

namespace cs {

  using OptionPtr = std::unique_ptr<class IOption>;

  class IOption {
  public:
    virtual ~IOption() noexcept;

    std::string help() const;
    void setHelp(const std::string& help);

    bool isLongFormat() const;
    void setLongFormat(const bool on);

    bool isRequired() const;
    void setRequired(const bool on);

    const char *name() const;

    bool isOption(const char *arg) const;
    bool parse(const char *arg);
    bool isValid() const;

    void printUsage(std::ostream *output) const;

    static bool isValidName(const char *s);

  protected:
    IOption(const std::string& name) noexcept;

  private:
    IOption() noexcept = delete;

    void initializePrefix();
    bool isValueOption() const;

    virtual const char *impl_defaultValue() const = 0;
    virtual bool impl_parse(const char *value) = 0;
    virtual bool impl_isValid() const = 0;

    std::string _help;
    bool _isLongFormat{false};
    bool _isRequired{true};
    std::string _name;
    std::string _prefix;

    template<typename DerivedT, typename... Args>
    friend OptionPtr make_option(const std::string& name, Args&&... args);
  };

  // cf. "C++ Core Guidelines", C.50
  template<typename DerivedT, typename... Args>
  OptionPtr make_option(const std::string& name, Args&&... args)
  {
    using ctor_tag = typename DerivedT::ctor_tag;
    OptionPtr ptr;
    try {
      ptr = std::make_unique<DerivedT>(ctor_tag{}, name, std::forward<Args>(args)...);
    } catch(...) {
      return OptionPtr{};
    }
    ptr->initializePrefix();
    return ptr;
  }

} // namespace cs
