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

#include <cs/Concurrent/SyncCondition.h>
#include <cs/Concurrent/SyncValue.h>
#include <cs/Concurrent/ThreadPool.h>
#include <cs/System/Random.h>
#include <cs/System/Time.h>
#include <cs/Text/PrintFormat.h>
#include <cs/Text/PrintUtil.h>

////// ThreadPool ////////////////////////////////////////////////////////////

using Pair = std::pair<int,unsigned int>;

void worker_pair(const Pair& p)
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
    pool.dispatch(std::bind(worker_pair, Pair(i, rng())));
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

void worker_sync(const std::size_t id, const Pair& p,
                 cs::SyncCondition<std::size_t>& sync)
{
  constexpr std::size_t ONE = 1;

  sync.wait(id);

  fprintf(stdout, "#%d, sleeping %ds...\n", p.first, p.second);
  fflush(stdout);

  const uint64_t beg = cs::tickCountMs();
  cs::sleep(p.second);
  const uint64_t end = cs::tickCountMs();

  cs::println("duration = %ms", end - beg);

  sync.notify_all(id + ONE);
}

void run_SyncValue()
{
  constexpr std::size_t INIT_ID = 0;

  cs::ThreadPool pool(4);

  cs::SyncCondition<std::size_t> sync(INIT_ID);

  std::size_t id{INIT_ID};
  cs::Random<unsigned int> rng(1, 3);
  for(int i = 0; i < 9; i++, id++) {
    pool.dispatch(std::bind(worker_sync, id, Pair(i, rng()), std::ref(sync)));
  }

  pool.finish();
}

//////////////////////////////////////////////////////////////////////////////

int main(int, char **)
{
  run_SyncValue();

  return EXIT_SUCCESS;
}
