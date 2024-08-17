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

#include <cstdio>
#include <cstdlib>

#include <cs/IO/BufferedReader.h>
#include <cs/IO/File.h>
#include <cs/System/FileSystem.h>
#include <cs/Text/PrintUtil.h>
#include <cs/Text/StringUtil.h>
#include <cs/Logging/Logger.h> // cf. Formatted Logging
#include <cs/Text/TextIO.h>

#include "SocketCAN.h"

void processLine(const cs::LoggerPtr& logger, const std::size_t lineno,
                 const std::string& line)
{
  if( line.empty() ) {
    logger->logWarning(lineno, u8"Ignoring empty line!");
    return;
  }

  if( line[0] != '(' ) {
    logger->logWarning(lineno, "Ignoring line with invalid start sequence \"%\"!", line[0]);
    return;
  }
}

int main(int /*argc*/, char **argv)
{
  cs::LoggerPtr logger = cs::Logger::make();

  const std::filesystem::path input("test_a.log");
  if( !cs::isFile(cs::reparent(argv[0], input)) ) {
    logger->logError("Input % not found!", input);
    return EXIT_FAILURE;
  }

  const std::list<std::string> lines = cs::readLines(input);
  if( lines.empty() ) {
    logger->logError("Unable to read input %!", input);
    return EXIT_FAILURE;
  }

  std::size_t lineno = 0;
  for(const std::string& line : lines) {
    lineno += 1;

    processLine(logger, lineno, line);
  }

  return EXIT_SUCCESS;
}
