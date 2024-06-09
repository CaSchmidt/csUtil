#pragma once

#include <string>

#include <cs/Core/csutil_config.h>
#include <cs/Core/Buffer.h>

namespace cs {

  ////// Implementation //////////////////////////////////////////////////////

  CS_UTIL_EXPORT Buffer toBuffer(const std::string_view& str);

  CS_UTIL_EXPORT std::string toString(const void *data, const std::size_t sizData,
                                      const char sep = '\0', const bool is_upper = false);

  ////// Helper //////////////////////////////////////////////////////////////

  inline std::string toString(const void *data, const std::size_t sizData,
                              const bool is_upper)
  {
    return toString(data, sizData, '\0', is_upper);
  }

  inline std::string toString(const Buffer& buffer,
                              const char sep = '\0', const bool is_upper = false)
  {
    return toString(buffer.data(), buffer.size(), sep, is_upper);
  }

  inline std::string toString(const Buffer& buffer,
                              const bool is_upper)
  {
    return toString(buffer, '\0', is_upper);
  }

} // namespace cs
