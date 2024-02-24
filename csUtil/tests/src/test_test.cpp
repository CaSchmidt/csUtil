#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <numeric>
#include <queue>
#include <random>
#include <semaphore>
#include <thread>
#include <tuple>
#include <utility>

#include <cs/Concurrent/ThreadPool.h>
#include <cs/System/Random.h>
#include <cs/System/Time.h>
#include <cs/Text/PrintFormat.h>
#include <cs/Text/PrintUtil.h>

////// ThreadPool ////////////////////////////////////////////////////////////

using Pair = std::pair<int,unsigned int>;

void work(const Pair& p)
{
#if 0
  fprintf(stdout, "#%d, sleeping %ds...\n", p.first, p.second);
  fflush(stdout);
#endif

  cs::sleep(p.second);

#if 1
  fprintf(stdout, "#%d, slept %ds\n", p.first, p.second);
  fflush(stdout);
#endif
}

void run_ThreadPool()
{
  cs::ThreadPool pool(4);

  cs::Random<unsigned int> rng(1, 5);
  for(int i = 0; i < 9; i++) {
    pool.dispatch(std::bind(work, Pair(i, rng())));
  }

  int value = 4;
  pool.dispatch(std::bind([](int& x, const int inc) -> void {
    x += inc;
  }, std::ref(value), 3));

#if 0
  cs::sleep(2);
  pool.clear();
#endif

  pool.finish();

  printf("value = %d\n", value);

  printf("valid = %d\n", int(pool.isValid()));
}

//////////////////////////////////////////////////////////////////////////////

int main(int, char **)
{
  run_ThreadPool();

  return EXIT_SUCCESS;
}
