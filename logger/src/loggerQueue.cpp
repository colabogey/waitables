#include <loggerQueue.h>

loggerRecord loggerQueue::pop() {
    std::unique_lock<std::mutex> lock(m_qmutex);
    m_qcv.wait(lock, [this] { return !m_q.empty() ; });
    loggerRecord lr = m_q.front();
    m_q.pop_front();
    return lr;
}

void loggerQueue::push(loggerRecord lr) { 
    const std::lock_guard<std::mutex> lock(m_qmutex);
    m_q.push_back(lr); 
    m_qcv.notify_one();
}

bool loggerQueue::isEmpty() { 
    const std::lock_guard<std::mutex> lock(m_qmutex);
    return m_q.empty(); 
}

size_t loggerQueue::size() { 
    const std::lock_guard<std::mutex> lock(m_qmutex);
    return m_q.size(); 
}

void loggerQueue::clear() { 
    const std::lock_guard<std::mutex> lock(m_qmutex);
    return m_q.clear(); 
}
