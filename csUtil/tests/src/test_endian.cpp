#include <cstdio>
#include <cstdlib>
#include <cstdint>

#include <iomanip>
#include <iostream>

#include <csUtil/csEndian.h>

template<typename T>
void print(const T& from, const T& to)
{
  std::cout
      << "0x" << std::setfill('0') << std::setw(sizeof(T)*2) << std::hex << from
      << " -> "
      << "0x" << std::setfill('0') << std::setw(sizeof(T)*2) << std::hex << to
      << std::endl;
}

template<typename T>
T do_swap(const T& value)
{
  const T swapped = cs::swap<true>(value);
  // print(value, swapped);
  return swapped;
}

uint16_t test_swap(const uint16_t& value)
{
  return do_swap(value);
}

uint32_t test_swap(const uint32_t& value)
{
  return do_swap(value);
}

uint64_t test_swap(const uint64_t& value)
{
  return do_swap(value);
}

const uint16_t value16{0x0102};
const uint32_t value32{0x01020304};
const uint64_t value64{0x0102030405060708};

volatile uint16_t swapped16;
volatile uint32_t swapped32;
volatile uint64_t swapped64;

int main(int /*argc*/, char ** /*argv*/)
{
  swapped16 = test_swap(value16);
  swapped32 = test_swap(value32);
  swapped64 = test_swap(value64);

  return EXIT_SUCCESS;
}
