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

#include <cstddef>

#include <exception>
#include <string>

#include <cs/Core/csutil_config.h>
#include <cs/Lexer/Token.h>

namespace cs {

  namespace parse_exception {

    class CS_UTIL_EXPORT base_exception : public std::exception {
    public:
      base_exception(const std::size_t line) noexcept;
      ~base_exception() noexcept;

      base_exception(const base_exception&) noexcept;
      base_exception& operator=(const base_exception&) noexcept;

      std::size_t line() const;

    private:
      base_exception() noexcept = delete;

      std::size_t _line{};
    };

    class CS_UTIL_EXPORT unexpected_token : public base_exception {
    public:
      unexpected_token(const TokenPtr& token,
                       const TokenNamesPtr& names) noexcept;
      unexpected_token(const tokenid_t expected, const TokenPtr& unexpected,
                       const TokenNamesPtr& names) noexcept;
      ~unexpected_token() noexcept;

      unexpected_token(const unexpected_token&) noexcept;
      unexpected_token& operator=(const unexpected_token&) noexcept;

      const char *what() const noexcept;

    private:
      unexpected_token() noexcept = delete;

      std::string _what;
    };

    class CS_UTIL_EXPORT unexpected_token_value : public base_exception {
    public:
      unexpected_token_value(const TokenPtr& token,
                             const TokenNamesPtr& names) noexcept;
      ~unexpected_token_value() noexcept;

      unexpected_token_value(const unexpected_token_value&) noexcept;
      unexpected_token_value& operator=(const unexpected_token_value&) noexcept;

      const char *what() const noexcept;

    private:
      unexpected_token_value() noexcept = delete;

      std::string _what;
    };

  } // namespace parse_exception

} // namespace cs
