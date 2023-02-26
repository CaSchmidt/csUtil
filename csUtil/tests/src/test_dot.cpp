#include <cstdio>
#include <cstdlib>
#include <cstddef>

#include <numeric>
#include <vector>

#include <cs/SIMD/SIMD.h>
#include <cs/SIMD/SIMD128.h>

using real_T = double;
using NumVec = std::vector<real_T>;

template<typename T>
void clflush(const std::vector<T>& v)
{
  using  size_type = typename std::vector<T>::size_type;
  using value_type = typename std::vector<T>::value_type;

  constexpr size_type SIZE_CACHE_LINE = 64;

  constexpr size_type NUM_ELEMS_LINE = SIZE_CACHE_LINE/sizeof(value_type);

  for(size_type i = 0; i < v.size(); i += NUM_ELEMS_LINE) {
    _mm_clflush(v.data() + i);
  }

  _mm_mfence();
}

extern "C" real_T dot(const real_T *a, const real_T *b, const std::size_t count)
{
#if 0
  real_T sum = 0;
  for(std::size_t i = 0; i < count; i++) {
    sum += a[i]*b[i];
  }
  return sum;
#else
  using SIMD = cs::simd128<real_T>;
  using OP = cs::simd::OP_mul<SIMD>;
  return cs::simd::impl_simd::reduce<SIMD,OP,true,true>(a, b, count);
#endif
}

real_T result = 0;

int main(int /*argc*/, char ** /*argv*/)
{
  constexpr std::size_t COUNT = 1000000;

  NumVec a;
  a.resize(COUNT, 1);
  NumVec b;
  b.resize(COUNT, 2);

  for(std::size_t i = 0; i < 1000; i++) {
    clflush(a);
    clflush(b);
    result += dot(a.data(), b.data(), COUNT);
  }

  return EXIT_SUCCESS;
}
