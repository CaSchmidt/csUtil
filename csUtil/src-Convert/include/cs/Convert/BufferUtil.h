#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <system_error>

#include <cs/Core/csutil_config.h>
#include <cs/Core/BufferView.h>

namespace cs {

  ////// Implementation //////////////////////////////////////////////////////

  CS_UTIL_EXPORT auto fromHexString(const std::string_view& str)
  -> std::expected<Buffer,std::errc>;

  CS_UTIL_EXPORT auto toHexString(const BufferView& data,
                                  const char sep = '\0',
                                  const bool is_upper = false)
  -> std::expected<std::string,std::errc>;

} // namespace cs
