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

#include <cs/Lexer/Lexer.h>
#include <cs/Lexer/Scanners/CharLiteral.h>
#include <cs/Lexer/Scanners/CIdentifier.h>
#include <cs/Lexer/Scanners/CIntegral.h>
#include <cs/Lexer/Scanners/CReal.h>
#include <cs/Lexer/Scanners/CString.h>
#include <cs/Lexer/Scanners/KeyWordScanner.h>

namespace cs {

  template<typename CharT>
  struct LexerContext {
    using char_type = CharT;

    using Lexer = Lexer<char_type>;

    template<typename T>
    using CIntegralScanner = CIntegralScanner<char_type,T>;

    template<typename T>
    using CRealScanner = CRealScanner<char_type,T>;

    using CharLiteralScanner = CharLiteralScanner<char_type>;
    using CIdentifierScanner = CIdentifierScanner<char_type>;
    using CStringScanner     = CStringScanner<char_type>;
    using KeyWordScanner     = KeyWordScanner<char_type>;
  };

} // namespace cs
