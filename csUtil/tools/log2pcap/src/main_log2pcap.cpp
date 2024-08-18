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
#include <cs/System/Time.h>
#include <cs/Text/PrintFormat.h>
#include <cs/Text/PrintUtil.h>
#include <cs/Text/StringUtil.h>
#include <cs/Logging/Logger.h> // cf. Formatted Logging
#include <cs/Text/StringValue.h>
#include <cs/Text/TextIO.h>

#include "SocketCAN.h"

struct LineInfo {
  LineInfo()
  {
  }

  bool isValid() const
  {
    return !device.empty()  &&  time.isValid();
  }

  std::string device;
  canid_t     id{0};
  bool        is_canfd{false};
  cs::TimeVal time{-1};
};

namespace parser {

  namespace chr = std::chrono;

  using      seconds_t = chr::seconds::rep;
  using microseconds_t = chr::microseconds::rep;

  using size_type = std::string::size_type;

  constexpr size_type NPOS = std::string::npos;
  constexpr size_type  ONE = 1;

  std::string_view make_view(const std::string& str,
                             const size_type beg, const size_type end)
  {
    return !str.empty()  &&  beg <= end  &&  end <= str.size()
        ? std::string_view(str.data() + beg, end - beg)
        : std::string_view();
  }

  cs::TimeVal parseTime(const std::string_view& str)
  {
    const cs::TimeVal ERROR_VAL(-1);

    const size_type idxDot = str.find('.');
    if( idxDot == NPOS ) {
      return ERROR_VAL;
    }

    bool ok = false;

    const chr::seconds secs{cs::toValue<seconds_t>(str.substr(0, idxDot), &ok)};
    if( !ok ) {
      return ERROR_VAL;
    }

    const chr::microseconds usecs{cs::toValue<microseconds_t>(str.substr(idxDot + ONE), &ok)};
    if( !ok ) {
      return ERROR_VAL;
    }

    return cs::TimeVal(secs, usecs);
  }

  LineInfo parseLine(const cs::LoggerPtr& logger,
                     const std::size_t lineno, const std::string& line)
  {
    LineInfo info;
    bool ok = false;

    // (0) Sanity Check ////////////////////////////////////////////////////////

    if( line.empty() ) {
      logger->logWarning(lineno, u8"Ignoring empty line!");
      return LineInfo();
    }

    if( line[0] != '(' ) {
      logger->logWarning(lineno, "Ignoring line with invalid start sequence \"%\"!", line[0]);
      return LineInfo();
    }

    // (1) Time Stamp //////////////////////////////////////////////////////////

    const size_type begTim = 1;
    const size_type endTim = line.find(')', begTim);
    if( endTim == NPOS ) {
      logger->logError(lineno, u8"Incomplete time stamp!");
      return LineInfo();
    }

    const std::string_view timeStr = make_view(line, begTim, endTim);
    info.time = parseTime(timeStr);
    if( !info.time.isValid() ) {
      logger->logError(lineno, "Invalid time stamp \"%\"!", timeStr);
      return LineInfo();
    }

    // (2) Device //////////////////////////////////////////////////////////////

    const size_type begDev = line.find_first_not_of(' ', endTim + ONE);
    if( begDev == NPOS ) {
      logger->logError(lineno, u8"No device found!");
      return LineInfo();
    }

    const size_type endDev = line.find(' ', begDev + ONE);
    if( endDev == NPOS ) {
      logger->logError(lineno, u8"Invalid device separator!");
      return LineInfo();
    }

    info.device = line.substr(begDev, endDev - begDev);

    // (3) Message ID ////////////////////////////////////////////////////////

    const size_type begId = line.find_first_not_of(' ', endDev + ONE);
    if( begId == NPOS ) {
      logger->logError(lineno, u8"No ID found!");
      return LineInfo();
    }

    const size_type endId = line.find('#', begId + ONE);
    if( endId == NPOS ) {
      logger->logError(lineno, u8"Invalid ID separator!");
      return LineInfo();
    }

    const std::string_view idStr = make_view(line, begId, endId);
    info.id = cs::toValue<canid_t>(idStr, &ok, 16);
    if( !ok ) {
      logger->logError(lineno, "Invalid ID string \"%\"!", idStr);
      return LineInfo();
    }

    return info;
  }

} // namespace parser

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

    const LineInfo info = parser::parseLine(logger, lineno, line);
    if( !info.isValid() ) {
      continue;
    }

#if 0
    if( lineno <= 10 ) {
      cs::println("%: %% % %", lineno,
                  info.time.secs(), info.time.usecs(), info.device, cs::hexf(info.id));
    }
#endif
  }

  return EXIT_SUCCESS;
}
