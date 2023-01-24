#include <cstdint>
#include <future>

#include <gtest/gtest.h>

#include <common/thread_pool/thread_pool.h>

TEST(ThreadPool, alarm)
{
    auto result = ::remotePortMapper::ThreadPool::create();
    ASSERT_TRUE(result);
    auto threadPool
        = result.value<::std::shared_ptr<::remotePortMapper::ThreadPool>>();

    // Test alarm.
    ::std::atomic<bool>       flag(false);
    ::std::condition_variable cond;
    ::std::mutex              mutex;
    auto alarm = threadPool->addAlarm(::std::chrono::steady_clock::now()
                                          + ::std::chrono::milliseconds(500),
                                      [&]() -> void {
                                          flag.store(true);
                                          cond.notify_one();
                                      });
    ::std::unique_lock lock(mutex);
    cond.wait_until(lock, ::std::chrono::steady_clock::now()
                              + ::std::chrono::milliseconds(100));
    ASSERT_FALSE(flag.load());
    cond.wait_until(lock, ::std::chrono::steady_clock::now()
                              + ::std::chrono::milliseconds(550));
    ASSERT_TRUE(flag.load());
    ASSERT_DEATH(alarm->alarm(), ".*");

    // Test cancel alarm.
    flag.store(false);
    alarm = threadPool->addAlarm(::std::chrono::steady_clock::now()
                                     + ::std::chrono::milliseconds(1000),
                                 [&]() -> void {
                                     flag.store(true);
                                     cond.notify_one();
                                 });
    ASSERT_TRUE(alarm->cancel());
    cond.wait_until(lock, ::std::chrono::steady_clock::now()
                              + ::std::chrono::milliseconds(1500));
    ASSERT_FALSE(flag.load());
    alarm->alarm();
    ASSERT_FALSE(flag.load());
}
