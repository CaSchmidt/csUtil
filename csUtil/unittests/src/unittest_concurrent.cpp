#include <iostream>
#include <mutex>
#include <numeric>
#include <random>
#include <thread>

#include <catch.hpp>

#include <cs/Concurrent/MapReduce.h>

namespace util {

  template<int MAX>
  class Delay {
  public:
    static_assert( MAX > 1 );

    Delay() noexcept
    {
      reset();
    }

    ~Delay() noexcept
    {
    }

    void operator()()
    {
      std::this_thread::sleep_for(std::chrono::seconds{random()});
    }

    int random()
    {
      std::lock_guard<std::mutex> guard(_mutex);
      return _dist(_gen);
    }

    void reset()
    {
      _dist.reset();

      std::random_device rd;
      _gen.seed(rd());
    }

  private:
    std::uniform_int_distribution<int> _dist{1, MAX};
    std::mt19937 _gen{};
    std::mutex _mutex{};
  };

  Delay<5> delay{};

  struct Increment {
    Increment() noexcept
    {
    }

    void operator()(int& i)
    {
      delay();

      const int old = i;
      i += 1;
      printf("%d -> %d\n", old, i); fflush(stdout);
    }
  };

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

  template<typename InputIt>
  void init(InputIt first, InputIt last, const int value1)
  {
    std::iota(first, last, value1);
  }

  void init(int *ptr, const std::size_t siz, const int value1)
  {
    init(ptr, ptr + siz, value1);
  }

  template<typename InputIt>
  bool isEqualSequence(InputIt first, InputIt last, int value1)
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

  template<typename InputIt>
  void print(InputIt first, InputIt last, const char *prefix = nullptr)
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

      auto f = cs::map(4, v.begin(), v.end(), util::Increment());
      f.get();

      // cs::blockingMap(4, v.cbegin(), v.cend(), util::Print());

      util::print(v.begin(), v.end(), "vnew = ");

      REQUIRE( util::isEqualSequence(v.begin(), v.end(), 2) );
    }

    std::cout << "---------------------------------------------" << std::endl;

    {
      int a[COUNT];
      util::init(a, COUNT, 1);
      util::print(a, COUNT, "aold = ");

      cs::blockingMap(4, a, a + COUNT, util::Increment());

      // cs::blockingMap(4, a, a + COUNT, util::Print());

      util::print(a, COUNT, "anew = ");

      REQUIRE( util::isEqualSequence(a, COUNT, 2) );
    }
  }

} // namespace test_map