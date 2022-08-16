#include <cs/Core/Endian.h>

namespace test_integral {

  uint16_t test_swap(const uint16_t& value)
  {
    return cs::swap(value);
  }

  uint32_t test_swap(const uint32_t& value)
  {
    return cs::swap(value);
  }

  uint64_t test_swap(const uint64_t& value)
  {
    return cs::swap(value);
  }

} // namespace test_integral
