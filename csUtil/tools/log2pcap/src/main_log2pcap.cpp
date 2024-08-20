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

#include <cs/Core/ByteArray.h>
#include <cs/IO/BufferedReader.h>
#include <cs/IO/File.h>
#include <cs/Math/Numeric.h>
#include <cs/System/FileSystem.h>
#include <cs/System/Time.h>
#include <cs/Text/PrintFormat.h>
#include <cs/Text/PrintUtil.h>
#include <cs/Text/StringUtil.h>
#include <cs/Logging/Logger.h> // cf. Formatted Logging
#include <cs/Text/StringValue.h>
#include <cs/Text/TextIO.h>

#include "SocketCAN.h"

using LineData = cs::ByteArray<CANFD_MAX_DLEN>;

struct LineInfo {
  LineInfo()
  {
    data.fill(0);
  }

  bool isValid() const
  {
    return !device.empty()  &&  time.isValid();
  }

  LineData    data;
  std::string device;
  uint8_t     fdflags{0};
  canid_t     id{0};
  bool        is_canfd{false};
  bool        is_rtr{false};
  uint8_t     len{0};
  cs::TimeVal time{-1};
};

namespace parser {

  using ConstStringIter = std::string::const_iterator;

  constexpr auto lambda_is_space()
  {
    return [](const char ch) -> bool {
      return ch == ' ';
    };
  }

  bool parseData(LineInfo& result, ConstStringIter& first, const ConstStringIter& last,
                 const cs::LoggerPtr& logger, const std::size_t lineno)
  {
    constexpr std::size_t TWO = 2;

    result.data.fill(0);

    std::size_t count = 0;
    for(; first != last; ++count, ++first) {
      const bool is_even = cs::isEven(count);

      if( !cs::isHexDigit(*first) ) {
        if( is_even ) {
          return true;
        } else {
          logger->logError(lineno, u8"Incomplete data!");
          return false;
        }

      } else {
        const std::size_t idxData = count/TWO;
        if( idxData >= result.data.size() ) {
          logger->logError(lineno, u8"Data buffer exceeded!");
          return false;
        }

        result.data[idxData] |= cs::fromHexChar(*first);
        if( is_even ) {
          result.data[idxData] <<= 4;
        } else {
          result.len++;
        }
      }
    } // For each character

    if( cs::isOdd(count) ) {
      logger->logError(lineno, u8"Incomplete data!");
      return false;
    }

    return true;
  }

  bool parseDevice(std::string& result, ConstStringIter& first, const ConstStringIter& last,
                   const cs::LoggerPtr& logger, const std::size_t lineno)
  {
    const ConstStringIter begDev = std::find_if_not(first, last, lambda_is_space());
    if( begDev == last ) {
      logger->logError(lineno, u8"Missing device declaration!");
      return false;
    }

    const ConstStringIter endDev = std::find(begDev, last, ' ');
    if( endDev == last ) {
      logger->logError(lineno, u8"Invalid device separator!");
      return false;
    }

    first = endDev;

    result.assign(begDev, endDev);

    return true;
  }

  bool parseId(canid_t& result, ConstStringIter& first, const ConstStringIter& last,
               const cs::LoggerPtr& logger, const std::size_t lineno)
  {
    const ConstStringIter begId = std::find_if_not(first, last, lambda_is_space());
    if( begId == last ) {
      logger->logError(lineno, u8"Missing message ID!");
      return false;
    }

    const ConstStringIter endId = std::find(begId, last, '#');
    if( endId == last ) {
      logger->logError(lineno, u8"Invalid ID separator!");
      return false;
    }

    bool ok = false;
    const std::string_view idStr(begId, endId);
    result = cs::toValue<canid_t>(idStr, &ok, 16);
    if( !ok ) {
      logger->logError(lineno, "Invalid ID string \"%\"!", idStr);
      return false;
    }

    first = endId;
    ++first; // NOTE: consider '#' part of the ID

    return true;
  }

  cs::TimeVal parseTime(const std::string_view& str)
  {
    namespace chr = std::chrono;

    using      seconds_t = chr::seconds::rep;
    using microseconds_t = chr::microseconds::rep;

    using size_type = std::string_view::size_type;

    constexpr size_type NPOS = std::string_view::npos;
    constexpr size_type  ONE = 1;

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

  bool parseTime(cs::TimeVal& result, ConstStringIter& first, const ConstStringIter& last,
                 const cs::LoggerPtr& logger, const std::size_t lineno)
  {
    if( *first != '(' ) {
      logger->logError(lineno, u8"Missing time stamp!");
      return false;
    }

    ++first; // parse '('

    const ConstStringIter endTim = std::find(first, last, ')');
    if( endTim == last ) {
      logger->logError(lineno, u8"Incomplete time stamp!");
      return false;
    }

    const std::string_view timeStr(first, endTim);
    result = parseTime(timeStr);
    if( !result.isValid() ) {
      logger->logError(lineno, "Invalid time stamp \"%\"!", timeStr);
      return false;
    }

    first = endTim;
    ++first; // parse ')'

    return true;
  }

  bool parseType(LineInfo& result, ConstStringIter& first, const ConstStringIter& last,
                 const cs::LoggerPtr& logger, const std::size_t lineno)
  {
    constexpr auto INVALID_HEXCHAR = std::numeric_limits<cs::byte_t>::max();

    result.fdflags = 0;
    result.is_canfd = false;
    result.is_rtr = false;
    result.len = 0;

    // (1) Message Type //////////////////////////////////////////////////////

    // NOTE: No message type for empty CAN 2.0 messages!
    if( first == last ) {
      return true;
    }

    if(        *first == '#' ) {
      result.is_canfd = true;
      ++first;

    } else if( *first == 'R' ) {
      result.is_rtr = true;
      ++first;

    } else if( cs::isHexDigit(*first) ) {
      return true;

    } else {
      logger->logError(lineno, "Invalid message type \"%\"!", *first);
      return false;

    }

    // (2) Message Extra /////////////////////////////////////////////////////

    if( first == last ) {
      logger->logError(lineno, u8"Missing message extra!");
      return false;
    }

    const uint8_t extra = cs::fromHexChar(*first);
    if( extra == INVALID_HEXCHAR ) {
      logger->logError(lineno, "Invalid message extra \"%\"!", *first);
      return false;
    }

    if(        result.is_canfd ) {
      result.fdflags = extra;
    } else if( result.is_rtr ) {
      result.len = extra;
    }

    ++first;

    return true;
  }

  LineInfo parseLine(ConstStringIter first, const ConstStringIter& last,
                     const cs::LoggerPtr& logger, const std::size_t lineno)
  {
    LineInfo info;

    // (0) Sanity Check ////////////////////////////////////////////////////////

    if( first == last ) {
      logger->logWarning(lineno, u8"Ignoring empty line!");
      return LineInfo();
    }

    if( *first != '(' ) {
      logger->logWarning(lineno, "Ignoring line with invalid start sequence \"%\"!", *first);
      return LineInfo();
    }

    // (1) Time Stamp //////////////////////////////////////////////////////////

    if( !parseTime(info.time, first, last, logger, lineno) ) {
      return LineInfo();
    }

    // (2) Device //////////////////////////////////////////////////////////////

    if( !parseDevice(info.device, first, last, logger, lineno) ) {
      return LineInfo();
    }

    // (3) Message ID ////////////////////////////////////////////////////////

    if( !parseId(info.id, first, last, logger, lineno) ) {
      return LineInfo();
    }

    // (4) Message Type: CAN 2.0, RTR, CAN FD ////////////////////////////////

    if( !parseType(info, first, last, logger, lineno) ) {
      return LineInfo();
    }

    // (5) Parse Data ////////////////////////////////////////////////////////

    if( !info.is_rtr  &&  !parseData(info, first, last, logger, lineno) ) {
      return LineInfo();
    }

    return info;
  }

  LineInfo parseLine(const cs::LoggerPtr& logger,
                     const std::size_t lineno, const std::string& line)
  {
    return parseLine(line.begin(), line.end(), logger, lineno);
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

#if 1
    if( lineno <= 40 ) {
      cs::print("%: %% % %#", lineno,
                info.time.secs(), info.time.usecs(), info.device, cs::hexf(info.id));
      if( info.is_canfd ) {
        cs::print("#%", cs::toHexChar<char,true>(info.fdflags));
      }
      if( info.is_rtr ) {
        cs::print("R%", cs::toHexChar<char,true>(info.len));
      } else {
        for(uint8_t i = 0; i < info.len; i++) {
          cs::print("%", cs::hexf(info.data[i], true));
        }
      }
      cs::println("");
    }
#endif
  } // For Each Line

  return EXIT_SUCCESS;
}
