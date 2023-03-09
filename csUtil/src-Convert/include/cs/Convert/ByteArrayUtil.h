#pragma once

#include <string>

#include <cs/Core/csutil_config.h>
#include <cs/Core/ByteArray.h>

namespace cs {

  ////// Implementation //////////////////////////////////////////////////////

  CS_UTIL_EXPORT ByteArray toByteArray(const char *str, const std::size_t lenStr);

  CS_UTIL_EXPORT std::string toString(const uint8_t *data, const std::size_t sizData,
                                      const char fill = '\0', const bool is_upper = false);

  ////// Helper //////////////////////////////////////////////////////////////

  inline ByteArray toByteArray(const std::string& s)
  {
    return toByteArray(s.data(), s.length());
  }

  inline std::string toString(const uint8_t *data, const std::size_t sizData,
                              const bool is_upper)
  {
    return toString(data, sizData, '\0', is_upper);
  }

  inline std::string toString(const ByteArray& a,
                              const char fill = '\0', const bool is_upper = false)
  {
    return toString(a.data(), a.size(), fill, is_upper);
  }

  inline std::string toString(const ByteArray& a,
                              const bool is_upper)
  {
    return toString(a, '\0', is_upper);
  }

} // namespace cs
