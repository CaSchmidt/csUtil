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

#include <memory>
#include <tuple>

#include <cs/Core/csutil_config.h>

namespace cs {

  using RunnablePtr = std::unique_ptr<class Runnable>;

  class CS_UTIL_EXPORT Runnable {
  public:
    Runnable() noexcept;
    virtual ~Runnable() noexcept;

    virtual void run() = 0;
  };

  ////// Generic Runnable ////////////////////////////////////////////////////

  template<typename Func, typename... Args>
  class GenericRunnable : public Runnable {
  public:
    GenericRunnable(Func&& func, Args&&... args) noexcept
      : _func{std::forward<Func>(func)}
      , _args{std::forward_as_tuple(std::forward<Args>(args)...)}
    {
    }

    ~GenericRunnable() noexcept
    {
    }

    void run()
    {
      std::apply(_func, _args);
    }

  private:
    Func&&                _func;
    std::tuple<Args&&...> _args;
  };

  template<typename Func, typename... Args>
  RunnablePtr make_runnable(Func&& func, Args&&... args)
  {
    return std::make_unique<GenericRunnable<Func,Args...>>(std::forward<Func>(func),
                                                           std::forward<Args>(args)...);
  }

} // namespace cs
