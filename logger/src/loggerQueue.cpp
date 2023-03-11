/* Copyright 2022-Present Steve Yackey

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

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
