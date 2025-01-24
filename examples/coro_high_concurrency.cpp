#include <chrono>
#include <cstdio>
#include <memory>
#include <thread>

#include "coro/coro.hpp"

static void ManySmallTask()
{
    auto scheduler = coro::io_scheduler::make_shared(coro::io_scheduler::options{
        .thread_strategy = coro::io_scheduler::thread_strategy_t::spawn,
        .pool            = coro::thread_pool::options{.thread_count = 5}});

    std::atomic_int32_t total = 0;

    auto increment = [](std::atomic_int32_t* count) -> coro::task<void>
    {
        // NOTE(breakds): There are quite some mysterious issues regarding
        // this behavior here.
        //
        // 1. Without adding the yield_for here, there is a very high possibility
        //    that it will drop some tasks. With te `yield_for`, the possibility is
        //    much lower.
        //
        // 2. With a single thread, if every tasks `yield_for` 50 seconds it would
        //    take a very long time to finish all this. However, in this case all
        //    1000000 tasks finished in
        ++(*count);
        co_return;
    };

    for (int i = 0; i < 1000000; ++i)
    {
        scheduler->schedule(increment(&total));
    }

    while (true)
    {
        printf("total = %d\n", total.load());
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char** argv)
{
    ManySmallTask(); // buggy
}
