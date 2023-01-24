#include <common/logger/logger.h>

#include <common/thread_pool/thread_pool.h>

namespace remotePortMapper {

/**
 * @brief       Constructor.
 */
ThreadPool::ThreadPool(::std::size_t workers) : m_running(true)
{
    for (::std::size_t i = 0; i < workers; ++i) {
        m_workers.push_back(::std::thread(&ThreadPool::workerThread, this));
    }

    m_alarmThread = ::std::thread(&ThreadPool::alarmThread, this);

    this->setInitializeResult(Result<void, Error>::makeOk());
    log_info("ThreadPool at " << this << " initialized.");
}

/**
 * @brief       Destructor.
 */
ThreadPool::~ThreadPool()
{
    log_info("ThreadPool at " << this << " destroying.");
    // Change status.
    m_running = false;
    ::std::atomic_thread_fence(::std::memory_order_seq_cst);

    // Awake all.
    m_alarmCond.notify_one();
    m_taskQueueCond.notify_all();

    // Join.
    m_alarmThread.join();
    for (auto &worker : m_workers) {
        worker.join();
    }

    log_info("ThreadPool at " << this << " destroyed.");
}

/**
 * @brief       Add task.
 */
void ThreadPool::addTask(Task task)
{
    ::std::unique_lock lock(m_taskQueueLock);
    m_taskQueue.push(::std::move(task));
    m_taskQueueCond.notify_one();
}

/**
 * @brief       Add alarm.
 */
::std::shared_ptr<ThreadPool::AsyncAlarm>
    ThreadPool::addAlarm(::std::chrono::steady_clock::time_point timepoint,
                         Task                                    task)
{
    ::std::unique_lock lock(m_alarmLock);
    auto               iter = m_alarmMap.find(timepoint);
    if (iter == m_alarmMap.end()) {
        // Create a new set.
        auto result = m_alarmMap.emplace(::std::make_pair(
            ::std::chrono::steady_clock::time_point(timepoint),
            ::std::set<::std::shared_ptr<ThreadPool::AsyncAlarm>>()));
        iter        = result.first;
    }

    // Add alarm.
    auto result = AsyncAlarm::create(timepoint, ::std::move(task),
                                     this->shared_from_this());
    auto alarm  = result.value<::std::shared_ptr<AsyncAlarm>>();
    iter->second.insert(alarm);
    m_alarmCond.notify_one();

    return alarm;
}

/**
 * @brief       Remove alarm.
 */
void ThreadPool::removeAlarm(::std::shared_ptr<AsyncAlarm> &alarm)
{
    ::std::unique_lock lock(m_alarmLock);
    auto               iter = m_alarmMap.find(alarm->timepoint());
    if (iter == m_alarmMap.end()) {
        return;
    }

    iter->second.erase(alarm);
    m_alarmCond.notify_one();
}

/**
 * @brief   Alarm thread function.
 */
void ThreadPool::alarmThread()
{
    while (true) {
        ::std::unique_lock lock(m_alarmLock);

        // Check empty.
        if (m_alarmMap.empty()) {
            if (m_running) {
                // Wait.
                m_alarmCond.wait(lock);
                continue;
            } else {
                break;
            }
        }

        // Check time.
        auto currentTime = ::std::chrono::steady_clock::now();
        auto iter        = m_alarmMap.begin();
        if (currentTime >= iter->first) {
            // Alarm.
            auto alarms = ::std::move(iter->second);
            m_alarmMap.erase(iter->first);
            lock.unlock();
            for (auto &alarm : alarms) {
                this->addTask(AlarmTask(alarm));
            }
            continue;

        } else {
            // Wait.
            m_alarmCond.wait_until(lock, iter->first);
            continue;
        }
    }
}

/**
 * @brief   Worker thread function.
 */
void ThreadPool::workerThread()
{
    while (true) {
        ::std::unique_lock lock(m_taskQueueLock);

        // Check empty.
        if (m_taskQueue.empty()) {
            if (m_running) {
                // Wait.
                m_taskQueueCond.wait(lock);
                continue;
            } else {
                break;
            }
        }

        // Take task.
        Task task = ::std::move(m_taskQueue.front());
        m_taskQueue.pop();

        // Run.
        lock.unlock();
        task();
    }
}

/**
 * @brief       Constructor.
 */
ThreadPool::AsyncAlarm::AsyncAlarm(
    ::std::chrono::steady_clock::time_point timepoint,
    Task                                    task,
    ::std::weak_ptr<ThreadPool>             threadPool) :
    m_timepoint(timepoint),
    m_task(::std::move(task)), m_status(Status::Ready), m_threadPool(threadPool)
{}

/**
 * @brief   Get timepoint to alarm.
 */
const ::std::chrono::steady_clock::time_point &
    ThreadPool::AsyncAlarm::timepoint() const
{
    return m_timepoint;
}

/**
 * @brief   Cancel the alarm.
 */
bool ThreadPool::AsyncAlarm::cancel()
{
    // Set status.
    Status expected = Status::Ready;
    bool   exchanged
        = m_status.compare_exchange_strong(expected, Status::Canceled);

    if (exchanged) {
        // Cancel.
        auto threadPool = m_threadPool.lock();
        if (threadPool != nullptr) {
            auto thisPtr = this->shared_from_this();
            threadPool->removeAlarm(thisPtr);
        }

        return true;
    } else {
        return m_status == Status::Canceled;
    }
}

/**
 * @brief   Run alarm task if not canceled or alarmed.
 */
void ThreadPool::AsyncAlarm::alarm()
{
    // Set status.
    Status expected = Status::Ready;
    bool   exchanged
        = m_status.compare_exchange_strong(expected, Status::Alarmed);

    if (exchanged) {
        m_task();
    }
}

/**
 * @brief       Constructor.
 */
ThreadPool::AlarmTask::AlarmTask(::std::shared_ptr<AsyncAlarm> alarm) :
    m_alarm(alarm)
{}

/**
 * @brief       Copy constructor.
 */
ThreadPool::AlarmTask::AlarmTask(const AlarmTask &alarm) :
    m_alarm(alarm.m_alarm)
{}

/**
 * @brief       Move constructor.
 */
ThreadPool::AlarmTask::AlarmTask(AlarmTask &&alarm) :
    m_alarm(::std::move(alarm.m_alarm))
{}

/**
 * @brief       Operator().
 */
void ThreadPool::AlarmTask::operator()()
{
    m_alarm->alarm();
}

} // namespace remotePortMapper
