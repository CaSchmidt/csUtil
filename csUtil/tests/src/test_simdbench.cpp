#include <cstdio>
#include <cstdlib>

#include <numeric>
#include <random>
#include <vector>

#include <cs/Core/Container.h>
#include <cs/SIMD/SIMD.h>
#include <cs/SIMD/SIMD128.h>
#include <cs/System/Time.h>
#include <cs/Text/PrintUtil.h>
#include <cs/Text/StringValue.h>

namespace util {

  template<typename vector_type>
  void flush_data(const vector_type& data)
  {
    using  size_type = typename vector_type::size_type;
    using value_type = typename vector_type::value_type;

    constexpr size_type CACHE_LINE_SIZE = 64;
    constexpr size_type STEP = CACHE_LINE_SIZE/sizeof(value_type);

    for(size_type i = 0; i < data.size(); i += STEP) {
      _mm_clflush(data.data() + i);
    }

    _mm_mfence();
  }

  template<typename vector_type>
  vector_type make_data(const typename vector_type::size_type size, const bool is_random = false,
                        const typename vector_type::value_type init = typename vector_type::value_type{0})
  {
    using  size_type = typename vector_type::size_type;
    using value_type = typename vector_type::value_type;

    using dist_type = std::uniform_int_distribution<>;

    vector_type result;

    if( !cs::resize(result, size) ) {
      return vector_type{};
    }

    if( is_random ) {
      std::random_device dev;
      std::mt19937       gen{dev()};
      dist_type         dist{0, 10};

      for(size_type i = 0; i < size; i++) {
        result[i] = value_type(dist(gen));
      }

    } else {
      for(size_type i = 0; i < size; i++) {
        result[i] = init;
      }
    }

    return result;
  }

  template<typename Func, typename vector_type>
  uint64_t do_bench(const char *name, Func func,
                    const vector_type& v,
                    const typename vector_type::size_type beg,
                    const typename vector_type::size_type end,
                    const uint64_t ref = 0)
  {
    if( v.empty() ) {
      return 0;
    }

    util::flush_data(v);

    const uint64_t start = cs::tickCountUs();
    const auto       sum = func(v, beg, end);
    const uint64_t  stop = cs::tickCountUs();

    const uint64_t dur = stop - start;
    const double   rat = (ref == 0
                          ? 1
                          : double(dur)/double(ref))*100.0;

    cs::println("%(%,%) = %, %us, ratio = %%",
                name, beg, end, sum, dur, cs::toString(rat, 'f', 2), '%');

    return dur;
  }

  template<typename Func, typename vector_type>
  uint64_t do_bench(const char *name, Func func,
                    const vector_type& v1, const vector_type& v2,
                    const typename vector_type::size_type beg,
                    const typename vector_type::size_type end,
                    const uint64_t ref = 0)
  {
    if( v1.empty()  ||  v2.empty() ) {
      return 0;
    }

    util::flush_data(v1);
    util::flush_data(v2);

    const uint64_t start = cs::tickCountUs();
    const auto       sum = func(v1, v2, beg, end);
    const uint64_t  stop = cs::tickCountUs();

    const uint64_t dur = stop - start;
    const double   rat = (ref == 0
                          ? 1
                          : double(dur)/double(ref))*100.0;

    cs::println("%(%,%) = %, %us, ratio = %%",
                name, beg, end, sum, dur, cs::toString(rat, 'f', 2), '%');

    return dur;
  }

} // namespace util

namespace simd_bench {

  using  value_type = float;
  using vector_type = std::vector<value_type>;
  using   size_type = typename vector_type::size_type;

  value_type sum_for(const vector_type& v,
                     const size_type beg, const size_type end)
  {
    value_type sum{0};
    for(size_type i = beg; i < end; i++) {
      sum += v[i];
    }
    return sum;
  }

  value_type sum_simd(const vector_type& v,
                      const size_type beg, const size_type end)
  {
    using SIMD = cs::SIMD128<value_type>;
    return cs::simd::sum<SIMD>(v.data() + beg, end - beg);
  }

  value_type dot_for(const vector_type& v1, const vector_type& v2,
                     const size_type beg, const size_type end)
  {
    value_type sum{0};
    for(size_type i = beg; i < end; i++) {
      sum += v1[i]*v2[i];
    }
    return sum;
  }

  value_type dot_simd(const vector_type& v1, const vector_type& v2,
                      const size_type beg, const size_type end)
  {
    using SIMD = cs::SIMD128<value_type>;
    if constexpr( cs::is_real_v<value_type> ) {
      return cs::simd::dot<SIMD>(v1.data() + beg, v2.data() + beg, end - beg);
    }
    return 0;
  }

  void benchmark(const int sel = 0)
  {
    cs::println("value_type = %%_t",
                cs::is_real_v<value_type> ? "real" : "int",
                sizeof(value_type)*8);

    constexpr size_type COUNT = cs::is_real32_v<value_type>
        ? 16'000'000
        : 100'000'000;

    if( sel <= 0  ||  sel == 1 ) {
      constexpr bool  IS_RANDOM = !cs::is_real_v<value_type>;
      constexpr value_type INIT = 1;

      const vector_type v = util::make_data<vector_type>(COUNT, IS_RANDOM, INIT);

      const uint64_t ref = util::do_bench("sum_for", sum_for, v, 0, COUNT);
      util::do_bench("sum_simd", sum_simd, v, 0, COUNT, ref);
    }

    if constexpr( cs::is_real_v<value_type> ) {
      if( sel <= 0  ||  sel == 2 ) {
        constexpr value_type init1 = 4.0;
        constexpr value_type init2 = 0.25;

        const vector_type v1 = util::make_data<vector_type>(COUNT, false, init1);
        const vector_type v2 = util::make_data<vector_type>(COUNT, false, init2);

        const uint64_t ref = util::do_bench("dot_for", dot_for, v1, v2, 0, COUNT);
        util::do_bench("dot_simd", dot_simd, v1, v2, 0, COUNT, ref);
      }
    }
  }

} // namespace simd_bench

int main(int /*argc*/, char ** /*argv*/)
{
  simd_bench::benchmark();

  return EXIT_SUCCESS;
}
