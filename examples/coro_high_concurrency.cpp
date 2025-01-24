#include <chrono>
#include <cstdio>
#include <memory>
#include <thread>

#include "coro/coro.hpp"

static void ManySmallTask()
{
    int N = 1000000;

    auto scheduler = coro::io_scheduler::make_shared(coro::io_scheduler::options{
        .thread_strategy = coro::io_scheduler::thread_strategy_t::spawn,
        .pool            = coro::thread_pool::options{.thread_count = 2}});

    std::atomic_int32_t total = 0;

    auto increment = [](std::atomic_int32_t* count) -> coro::task<void>
    {
        ++(*count);
        co_return;
    };

    for (int i = 0; i < N; ++i)
    {
        scheduler->schedule(increment(&total));
    }

    while (true)
    {
        printf("total = %d\n", total.load());
        scheduler->print_pool_info();
        scheduler->print_container_info();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argc, char** argv)
{
    ManySmallTask(); // buggy
}
