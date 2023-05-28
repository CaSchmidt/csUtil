#pragma once

#include <string>

#include <cs/Core/csutil_config.h>
#include <cs/Core/Buffer.h>

namespace cs {

  ////// Implementation //////////////////////////////////////////////////////

  CS_UTIL_EXPORT Buffer toBuffer(const char *str, const std::size_t lenStr);

  CS_UTIL_EXPORT std::string toString(const void *data, const std::size_t sizData,
                                      const char fill = '\0', const bool is_upper = false);

  ////// Helper //////////////////////////////////////////////////////////////

  inline Buffer toBuffer(const std::string& s)
  {
    return toBuffer(s.data(), s.size());
  }

  inline std::string toString(const void *data, const std::size_t sizData,
                              const bool is_upper)
  {
    return toString(data, sizData, '\0', is_upper);
  }

  inline std::string toString(const Buffer& buffer,
                              const char fill = '\0', const bool is_upper = false)
  {
    return toString(buffer.data(), buffer.size(), fill, is_upper);
  }

  inline std::string toString(const Buffer& buffer,
                              const bool is_upper)
  {
    return toString(buffer, '\0', is_upper);
  }

} // namespace cs
