#pragma once

#include <chrono>
#include <condition_variable>
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <thread>

#include <common/utils/i_create_shared_function.h>

namespace remotePortMapper {

/**
 * @brief   Thread pool.
 */
class ThreadPool :
    public ::std::enable_shared_from_this<ThreadPool>,
    virtual public ICreateSharedFunc<ThreadPool>,
    virtual public ICreateSharedFunc<ThreadPool, ::std::size_t> {
    CREATE_SHARED(ThreadPool);
    CREATE_SHARED(ThreadPool, ::std::size_t);

  public:
    /**
     * @brief   Task to call.
     */
    using Task = ::std::function<void()>;

  private:
    /**
     * @brief       Asynchronous alarm object.
     */
    class AsyncAlarm;

    /**
     * @brief       Alarm task wrapper.
     */
    class AlarmTask;

  private:
    // Tasks.
    ::std::mutex m_taskQueueLock; ///< Lock for task queue.
    ::std::condition_variable
                       m_taskQueueCond; ///< Condition variable for task queue.
    ::std::queue<Task> m_taskQueue;     ///< Task queue.

    // Alarms.
    ::std::mutex              m_alarmLock; ///< Lock vor alarm.
    ::std::condition_variable m_alarmCond; ///< Condition variable for alarm.
    ::std::map<::std::chrono::steady_clock::time_point,
               ::std::set<::std::shared_ptr<AsyncAlarm>>>
                  m_alarmMap;    ///< Sorted alarms.
    ::std::thread m_alarmThread; ///< Thread to handle alarm.

    // Workers.
    ::std::vector<::std::thread> m_workers; ///< Workers.
    volatile bool                m_running; ///< Running flag.

  private:
    /**
     * @brief       Constructor.
     *
     * @param[in]   workers         Counnt of workers.
     */
    ThreadPool(::std::size_t workers
               = ::std::thread::hardware_concurrency() + 1);

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&)      = delete;

  public:
    /**
     * @brief       Destructor.
     */
    virtual ~ThreadPool();

  public:
    /**
     * @brief       Add task.
     */
    void addTask(Task task);

    /**
     * @brief       Add alarm.
     *
     * @param[in]   timepoint       Timepoint to alarm.
     * @param[in]   task            Task to run when alarm.
     *
     * @return      Alarm object.
     */
    ::std::shared_ptr<AsyncAlarm>
        addAlarm(::std::chrono::steady_clock::time_point timepoint, Task task);

  private:
    /**
     * @brief       Remove alarm.
     *
     * @param[in]   alarm       Alarm to remove.
     */
    void removeAlarm(::std::shared_ptr<AsyncAlarm> &alarm);

  private:
    /**
     * @brief   Alarm thread function.
     */
    void alarmThread();

    /**
     * @brief   Worker thread function.
     */
    void workerThread();
};

/**
 * @brief       Asynchronous alarm object.
 */
class ThreadPool::AsyncAlarm :
    public ::std::enable_shared_from_this<AsyncAlarm>,
    virtual public ICreateSharedFunc<AsyncAlarm,
                                     ::std::chrono::steady_clock::time_point,
                                     Task,
                                     ::std::weak_ptr<ThreadPool>> {
    CREATE_SHARED(AsyncAlarm,
                  ::std::chrono::steady_clock::time_point,
                  Task,
                  ::std::weak_ptr<ThreadPool>);

  private:
    /**
     * @brief   Alarm status.
     */
    enum class Status {
        Ready,   ///< Alarm is ready.
        Alarmed, ///< Alarm has been triggered.
        Canceled ///< Alarm has been canceled.
    };

  private:
    ::std::chrono::steady_clock::time_point
                                m_timepoint;  ///< Timepoint to alarm.
    Task                        m_task;       ///< Task.
    ::std::atomic<Status>       m_status;     ///< Status.
    ::std::weak_ptr<ThreadPool> m_threadPool; ///< Thread pool.

  private:
    /**
     * @brief       Constructor.
     *
     * @param[in]   timepoint   Timepoint to alarm.
     * @param[in]   task        Task.
     * @param[in]   threadPool  Thread pool.
     */
    AsyncAlarm(::std::chrono::steady_clock::time_point timepoint,
               Task                                    task,
               ::std::weak_ptr<ThreadPool>             threadPool);

    AsyncAlarm(const AsyncAlarm &) = delete;
    AsyncAlarm(AsyncAlarm &&)      = delete;

  public:
    /**
     * @brief       Destructor.
     */
    ~AsyncAlarm() = default;

  public:
    /**
     * @brief   Get timepoint to alarm.
     *
     * @return  Timepoint to alarm.
     */
    const ::std::chrono::steady_clock::time_point &timepoint() const;

    /**
     * @brief   Cancel the alarm.
     *
     * @return  \c true if canceled, \c false if alarmed.
     */
    bool cancel();

    /**
     * @brief   Run alarm task if not canceled or alarmed.
     */
    void alarm();
};

/**
 * @brief       Alarm task wrapper.
 */
class ThreadPool::AlarmTask {
  private:
    ::std::shared_ptr<AsyncAlarm> m_alarm; ///< Alarm.

  public:
    /**
     * @brief       Constructor.
     *
     * @param[in]   alarm       Alarm.
     */
    AlarmTask(::std::shared_ptr<AsyncAlarm> alarm);

    /**
     * @brief       Copy constructor.
     *
     * @param[in]   alarm       Alarm to copy.
     */
    AlarmTask(const AlarmTask &alarm);

    /**
     * @brief       Move constructor.
     *
     * @param[in]   alarm       Alarm to move.
     */
    AlarmTask(AlarmTask &&alarm);

    /**
     * @brief       Destructor.
     */
    ~AlarmTask() = default;

  public:
    /**
     * @brief       Operator().
     */
    void operator()();
};

} // namespace remotePortMapper
