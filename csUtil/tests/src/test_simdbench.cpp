#include <cstdio>
#include <cstdlib>

#include <numeric>
#include <random>
#include <vector>

#include <cs/Core/Container.h>
#include <cs/Core/PrintFormat.h>
#include <cs/Core/PrintUtil.h>
#include <cs/SIMD/SIMD.h>
#include <cs/SIMD/SIMD128.h>
#include <cs/System/Time.h>

namespace util {

  template<typename vector_type,
           typename  size_type = typename vector_type::size_type,
           typename value_type = typename vector_type::value_type>
  void flush_data(const vector_type& data)
  {
    constexpr size_type CACHE_LINE_SIZE = 64;
    constexpr size_type STEP = CACHE_LINE_SIZE/sizeof(value_type);

    for(size_type i = 0; i < data.size(); i += STEP) {
      _mm_clflush(data.data() + i);
    }

    _mm_mfence();
  }

  template<typename vector_type,
           typename  size_type = typename vector_type::size_type,
           typename value_type = typename vector_type::value_type>
  vector_type make_data(const size_type size, const bool is_random = false,
                        const value_type init = value_type{0})
  {
    using dist_type = std::uniform_int_distribution<>;

    vector_type result;

    if( !cs::resize(&result, size) ) {
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
                name, beg, end, sum, dur, cs::fixedf(rat, 2), '%');

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
                name, beg, end, sum, dur, cs::fixedf(rat, 2), '%');

    return dur;
  }

} // namespace util

namespace bench_int {

  using   size_type = std::size_t;
  using    sum_type = int64_t;
  using  value_type = int32_t;
  using vector_type = std::vector<value_type>;

  sum_type sum_for(const vector_type& data,
                   const size_type beg, const size_type end)
  {
    sum_type sum{0};
    for(size_type i = beg; i < end; i++) {
      sum += data[i];
    }
    return sum;
  }

  sum_type sum_accumulate(const vector_type& data,
                          const size_type beg, const size_type end)
  {
    const value_type *begIt = data.data() + beg;
    const value_type *endIt = data.data() + end;
    return std::accumulate(begIt, endIt, sum_type{0});
  }

  value_type sum_simd(const vector_type& data,
                      const size_type beg, const size_type end)
  {
    using SIMD = cs::simd128<value_type>;

    return cs::simd::sum<SIMD>(data.data() + beg, end - beg);
  }

  void benchmark()
  {
    constexpr size_type COUNT = 100'000'000;

    const vector_type data = util::make_data<vector_type>(COUNT, true);
    if( data.empty() ) {
      return;
    }

    const uint64_t ref = util::do_bench("sum_for", sum_for, data, 0, COUNT);
    util::do_bench("sum_accumulate", sum_accumulate, data, 0, COUNT, ref);
    util::do_bench("sum_simd", sum_simd, data, 0, COUNT, ref);
  }

} // namespace bench_int

namespace bench_real {

  using   size_type = std::size_t;
  using  value_type = float;
  using vector_type = std::vector<value_type>;

  value_type dot_for(const vector_type& v1, const vector_type& v2,
                     const size_type beg, const size_type end)
  {
    value_type result{0};
    for(size_type i = beg; i < end; i++) {
      result += v1[i]*v2[i];
    }
    return result;
  }

  value_type dot_simd(const vector_type& v1, const vector_type& v2,
                      const size_type beg, const size_type end)
  {
    using SIMD = cs::simd128<value_type>;

    return cs::simd::dot<SIMD>(v1.data() + beg, v2.data() + beg, end - beg);
  }

  void benchmark()
  {
    constexpr size_type COUNT = 16'000'000;

    constexpr value_type a = 4.0;
    constexpr value_type b = 0.25;

    const vector_type v1 = util::make_data<vector_type>(COUNT, false, a);
    const vector_type v2 = util::make_data<vector_type>(COUNT, false, b);
    if( v1.empty()  ||  v2.empty() ) {
      return;
    }

    const uint64_t ref = util::do_bench("dot_for", dot_for, v1, v2, 0, COUNT);
    util::do_bench("dot_simd", dot_simd, v1, v2, 0, COUNT, ref);
  }

} // namespace bench_real

int main(int /*argc*/, char ** /*argv*/)
{
  bench_real::benchmark();

  return EXIT_SUCCESS;
}
