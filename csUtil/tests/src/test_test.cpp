#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <numeric>
#include <semaphore>
#include <thread>

#include <cs/System/Time.h>

template<typename ...Args>
void print_sig(Args&&...)
{
  std::cout << __FUNCSIG__ << std::endl;
  std::cout.flush();
}

//////////////////////////////////////////////////////////////////////////////

namespace cs {

  namespace concurrent {

    namespace impl_concurrent {

      using Semaphore = std::counting_semaphore<std::_Semaphore_max>;

      template<typename UnaryFunction, typename T, typename ...Args>
      concept IsForEach = std::is_invocable_v<UnaryFunction,T,Args...>;

      template<typename UnaryFunction, typename T, typename ...Args>
      requires IsForEach<UnaryFunction,T,Args...>
      void InvokeForEach(Semaphore *sema, UnaryFunction func, T arg, Args&&... opts)
      {
        std::invoke(func, arg, std::forward<Args>(opts)...);
        sema->release();
      }

      inline unsigned int threadCount(const unsigned int want)
      {
        const auto have = std::thread::hardware_concurrency();
        return have >= 1  &&  want >= 1
            ? std::min(have, want)
            : 1;
      }

    } // namespace impl_concurrent

    template<typename UnaryFunction, typename InputIt, typename ...Args>
    void run(const unsigned int numThreadsWant, UnaryFunction func,
             InputIt first, InputIt last, Args&&... opts)
    {
      using namespace impl_concurrent;

      using value_type = decltype(*first);

      const unsigned int numThreadsHave = threadCount(numThreadsWant);

      Semaphore sema{numThreadsHave};
      for(; first != last; ++first) {
        sema.acquire();
        std::thread t{InvokeForEach<UnaryFunction,value_type,Args...>, &sema, func, std::ref(*first), std::forward<Args>(opts)...};
        t.detach();
      }

      // synchronize workers...
      for(unsigned int i = 0; i < numThreadsHave; i++) {
        sema.acquire();
      }
    }

  } // namespace concurrent

} // namespace cs

//////////////////////////////////////////////////////////////////////////////

using Value_T = unsigned int;
using  Values = std::list<Value_T>;

void print(const Values& values, const char *ident = "values")
{
  std::cout << ident << " =";
  for(const Value_T& value : values) {
    std::cout << " " << value;
  }
  std::cout << std::endl;
  std::cout.flush();
}

Values make_values(const std::size_t count)
{
  Values values;

  for(std::size_t i = 0; i < count; i++) {
    values.push_back(Value_T(i + 1));
  }

  return values;
}

int main(int, char **)
{
  constexpr auto f_sleep = [](const Value_T secs) -> void {
    if( secs < 1 ) {
      return;
    }
    printf("Sleeping for %ds...\n", secs); fflush(stdout);
    cs::sleep(secs);
    printf("Done %ds...\n", secs); fflush(stdout);
  };

  constexpr auto f_mod = [](Value_T& value, const Value_T delta) -> void {
    value += delta;
  };

  printf("thread count: %d\n", std::thread::hardware_concurrency());

  Values values = make_values(6);
  print(values);

  const auto beg = cs::tickCountMs();
#if 1
  cs::concurrent::run(4, f_sleep, values.cbegin(), values.cend());
#else
  cs::concurrent::run(4, f_mod, values.begin(), values.end(), 2);
#endif
  const auto end = cs::tickCountMs();
  std::cout << "duration: " << end - beg << "ms" << std::endl;

  print(values, "result");

  return EXIT_SUCCESS;
}
