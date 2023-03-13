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

#include <map>

#include <cs/CLI/BooleanOption.h>
#include <cs/CLI/IntegralOption.h>
#include <cs/CLI/StringOption.h>

namespace cs {

  using OptionsPtr = std::unique_ptr<class Options>;

  class Options {
  private:
    struct ctor_tag {
      ctor_tag() noexcept
      {
      }
    };

  public:
    using      OptionMap  = std::map<std::string,OptionPtr>;
    using      OptionIter = OptionMap::iterator;
    using ConstOptionIter = OptionMap::const_iterator;

    Options(const ctor_tag&) noexcept;
    ~Options() noexcept;

    void clear();

    bool add(OptionPtr& ptr);

    bool isValid(std::ostream *output) const;
    bool parse(int argc, char **argv, std::ostream *output);

    void printUsage(int argc, char **argv, std::ostream *output) const;

    void setLongFormat(const bool on);

    const IOption *get(const std::string& name) const;

    template<typename T>
    inline if_boolean_t<T> value(const std::string& name) const
    {
      return dynamic_cast<const BooleanOption*>(get(name))->value();
    }

    template<typename T>
    inline if_integral_t<T> value(const std::string& name) const
    {
      return dynamic_cast<const IntegralOption<T>*>(get(name))->value();
    }

    template<typename T>
    inline if_real_t<T> value(const std::string& /*name*/) const
    {
      // TODO...
    }

    template<typename T>
    inline std::enable_if_t<std::is_same_v<T,std::string>,T> value(const std::string& name) const
    {
      return dynamic_cast<const StringOption*>(get(name))->value();
    }

    static OptionsPtr make();

  private:
    OptionMap _options;
  };

} // namespace cs
