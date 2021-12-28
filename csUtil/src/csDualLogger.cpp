/****************************************************************************
** Copyright (c) 2021, Carsten Schmidt. All rights reserved.
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

#include "csUtil/csDualLogger.h"

////// public ////////////////////////////////////////////////////////////////

csDualLogger::csDualLogger(const csILogger *a, const csILogger *b) noexcept
  : _a(a)
  , _b(b)
{
}

csDualLogger::~csDualLogger() noexcept
{
}

void csDualLogger::logFlush() const
{
  _a->logFlush();
  _b->logFlush();
}

void csDualLogger::logText(const char8_t *s) const
{
  _a->logText(s);
  _b->logText(s);
}

void csDualLogger::logWarning(const char8_t *s) const
{
  _a->logWarning(s);
  _b->logWarning(s);
}

void csDualLogger::logWarning(const int lineno, const char8_t *s) const
{
  _a->logWarning(lineno, s);
  _b->logWarning(lineno, s);
}

void csDualLogger::logError(const char8_t *s) const
{
  _a->logError(s);
  _b->logError(s);
}

void csDualLogger::logError(const int lineno, const char8_t *s) const
{
  _a->logError(lineno, s);
  _b->logError(lineno, s);
}
