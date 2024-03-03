#include <array>
#include <iostream>
#include <numeric>

#include <catch.hpp>

#include <cs/Concurrent/Map.h>
#include <cs/Concurrent/MapReduce.h>
#include <cs/System/Random.h>
#include <cs/System/Time.h>
#include <cs/Text/PrintUtil.h>

inline constexpr std::size_t NUM_THREADS = 4;

namespace conc = cs::concurrent;

namespace util {

  class Delay {
  public:
    using Random = cs::Random<unsigned int>;

    using random_t = Random::result_type;

    Delay() noexcept
      : _random(1, 5)
    {
    }

    ~Delay() noexcept
    {
    }

    void operator()() const
    {
      auto thiz = const_cast<Delay*>(this);
      cs::sleep(thiz->random());
    }

    random_t random()
    {
      std::lock_guard<std::mutex> guard(_mutex);
      return _random();
    }

  private:
    std::mutex _mutex;
    Random     _random;
  };

  Delay delay;

  ////////////////////////////////////////////////////////////////////////////

  struct Increment {
    Increment() noexcept
    {
    }

    void operator()(int& i) const
    {
      delay();

      const int old = i;
      i += 1;
      printf("%d -> %d\n", old, i); fflush(stdout);
    }
  };

  ////////////////////////////////////////////////////////////////////////////

  struct Print {
    Print() noexcept
    {
    }

    void operator()(const int& i)
    {
      delay();

      printf("%d\n", i); fflush(stdout);
    }
  };

  ////////////////////////////////////////////////////////////////////////////

  int inc(int& i)
  {
    delay();

    const int old = i;
    i += 1;
    printf("%d -> %d\n", old, i); fflush(stdout);

    return -1;
  }

  int incTo(const int in)
  {
    delay();

    const int out = in + 1;
    printf("%d -> %d\n", in , out); fflush(stdout);

    return out;
  }

  ////////////////////////////////////////////////////////////////////////////

  void reduce(std::string& result, const int i)
  {
    result += std::to_string(i);
  }

  ////////////////////////////////////////////////////////////////////////////

  template<typename ForwardIt>
  int accumulate(ForwardIt first, ForwardIt last, const int init = 0)
  {
    return std::accumulate(first, last, init);
  }

  int accumulate(const int *ptr, const std::size_t siz, const int init = 0)
  {
    return accumulate(ptr, ptr + siz, init);
  }

  template<typename ForwardIt>
  void init(ForwardIt first, ForwardIt last, const int value1)
  {
    std::iota(first, last, value1);
  }

  void init(int *ptr, const std::size_t siz, const int value1)
  {
    init(ptr, ptr + siz, value1);
  }

  template<typename ForwardIt>
  bool isEqualSequence(ForwardIt first, ForwardIt last, int value1)
  {
    for(; first != last; ++first) {
      if( *first != value1++ ) {
        return false;
      }
    }
    return true;
  }

  bool isEqualSequence(const int *ptr, const std::size_t siz, const int value1)
  {
    return isEqualSequence(ptr, ptr + siz, value1);
  }

  template<typename ForwardIt>
  void print(ForwardIt first, ForwardIt last, const char *prefix = nullptr)
  {
    if( prefix != nullptr ) {
      std::cout << prefix;
    }

    if( first != last ) {
      std::cout << *first;
      ++first;
    }

    for(; first != last; ++first) {
      std::cout << ", " << *first;
    }

    std::cout << std::endl;

    std::cout.flush();
  }

  void print(const int *ptr, const std::size_t siz, const char *prefix = nullptr)
  {
    print(ptr, ptr + siz, prefix);
  }

} // namespace util

namespace test_map {

  TEST_CASE("Map sequence in-place.", "[map]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    constexpr std::size_t COUNT = 7;

    {
      std::vector<int> v(COUNT);
      util::init(v.begin(), v.end(), 1);
      util::print(v.begin(), v.end(), "vold = ");

      auto f = conc::mapAsync(NUM_THREADS,
                              v.begin(), v.end(),
                              &util::inc);
      f.get();

      // cs::blockingMap(NUM_THREADS, v.cbegin(), v.cend(), util::Print());

      util::print(v.begin(), v.end(), "vnew = ");

      REQUIRE( util::isEqualSequence(v.begin(), v.end(), 2) );
    }

    std::cout << "---------------------------------------------" << std::endl;

    {
      int a[COUNT];
      util::init(a, COUNT, 1);
      util::print(a, COUNT, "aold = ");

      conc::map(NUM_THREADS,
                a, a + COUNT,
                util::Increment());

      // cs::blockingMap(NUM_THREADS, a, a + COUNT, util::Print());

      util::print(a, COUNT, "anew = ");

      REQUIRE( util::isEqualSequence(a, COUNT, 2) );
    }

    std::cout << std::endl;
  }

  TEST_CASE("Map sequence to sorted sequence.", "[mapsorted]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    constexpr std::size_t COUNT = 7;
    constexpr std::size_t EXTRA = 3;
    const std::array<int,COUNT+EXTRA> data{1, 2, 3, 4, 5, 6, 7, 0, 0, 0};

    util::print(data.begin(), data.end(), "data = ");

    std::cout << "---------------------------------------------" << std::endl;

    {
      std::vector<int> v(COUNT + EXTRA, 0);

      conc::mapSorted(NUM_THREADS,
                      v.begin(), std::next(v.begin(), COUNT),
                      data.begin(), data.end(),
                      &util::incTo);

      util::print(v.begin(), v.end(), "vnew = ");

      REQUIRE( util::isEqualSequence(v.begin(), std::next(v.begin(), COUNT), 2) );
    }

    std::cout << "---------------------------------------------" << std::endl;

    {
      int a[COUNT + EXTRA] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

      auto f = conc::mapSortedAsync(NUM_THREADS,
                                    a, a + COUNT + EXTRA,
                                    data.begin(), std::next(data.begin(), COUNT),
                                    &util::incTo);
      f.get();

      util::print(a, COUNT + EXTRA, "anew = ");

      REQUIRE( util::isEqualSequence(a, COUNT, 2) );
    }

    std::cout << std::endl;
  }

  TEST_CASE("Map sequence to unsorted sequence.", "[mapunsorted]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    constexpr std::size_t COUNT = 7;
    const std::array<int,COUNT> data{1, 2, 3, 4, 5, 6, 7};

    util::print(data.begin(), data.end(), "data = ");

    std::cout << "---------------------------------------------" << std::endl;

    {
      std::vector<int> v;

      conc::mapUnsorted(NUM_THREADS,
                        std::back_inserter(v),
                        data.begin(), data.end(),
                        &util::incTo);

      util::print(v.begin(), v.end(), "vnew = ");

      REQUIRE( util::accumulate(v.begin(), v.end()) == 35 );
    }

    std::cout << "---------------------------------------------" << std::endl;

    {
      int a[COUNT];

      auto f = conc::mapUnsortedAsync(NUM_THREADS,
                                      a,
                                      data.begin(), data.end(),
                                      &util::incTo);
      f.get();

      util::print(a, COUNT, "anew = ");

      REQUIRE( util::accumulate(a, COUNT) == 35 );
    }

    std::cout << std::endl;
  }

} // namespace test_map

namespace test_mapreduce {

  TEST_CASE("Sorted map-reduce.", "[mapreducesorted]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    constexpr std::size_t COUNT = 7;
    const std::array<int,COUNT> data{1, 2, 3, 4, 5, 6, 7};

    util::print(data.begin(), data.end(), "data = ");

    std::cout << "---------------------------------------------" << std::endl;

    {
      const std::string reduced =
          conc::mapReduceSorted<std::string>(NUM_THREADS,
                                             data.begin(), data.end(),
                                             &util::incTo, &util::reduce);
      cs::println("reduced = %", reduced);

      REQUIRE( reduced == "2345678" );
    }

    std::cout << "---------------------------------------------" << std::endl;

    {
      auto f = conc::mapReduceSortedAsync<std::string>(NUM_THREADS,
                                                       data.begin(), data.end(),
                                                       &util::incTo, &util::reduce);
      const std::string reduced = f.get();
      cs::println("reduced = %", reduced);

      REQUIRE( reduced == "2345678" );
    }

    std::cout << std::endl;
  }

  TEST_CASE("Unsorted map-reduce.", "[mapreduceunsorted]") {
    std::cout << "*** " << Catch::getResultCapture().getCurrentTestName() << std::endl;

    constexpr std::size_t COUNT = 7;
    const std::array<int,COUNT> data{1, 2, 3, 4, 5, 6, 7};

    util::print(data.begin(), data.end(), "data = ");

    std::cout << "---------------------------------------------" << std::endl;

    {
      const std::string unsorted =
          conc::mapReduceUnsorted<std::string>(NUM_THREADS,
                                               data.begin(), data.end(),
                                               &util::incTo, &util::reduce);
      cs::println("reduced (unsorted) = %", unsorted);

      REQUIRE( unsorted.size() == data.size() );

      std::string sorted(unsorted);
      std::sort(sorted.begin(), sorted.end());
      cs::println("reduced (sorted) = %", sorted);

      REQUIRE( sorted == "2345678" );
    }

    std::cout << "---------------------------------------------" << std::endl;

    {
      auto f = conc::mapReduceUnsortedAsync<std::string>(NUM_THREADS,
                                                         data.begin(), data.end(),
                                                         &util::incTo, &util::reduce);
      const std::string unsorted = f.get();
      cs::println("reduced (unsorted) = %", unsorted);

      REQUIRE( unsorted.size() == data.size() );

      std::string sorted(unsorted);
      std::sort(sorted.begin(), sorted.end());
      cs::println("reduced (sorted) = %", sorted);

      REQUIRE( sorted == "2345678" );
    }

    std::cout << std::endl;
  }

} // namespace test_mapreduce
