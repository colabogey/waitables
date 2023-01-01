#include <sys/eventfd.h>
#include <unistd.h>
#include <string.h>
#include <vpgnixexception.h>
#include <waitable_event.h>

void waitable_event::create_event(int id) {
    m_id = id;
    m_fd = eventfd(0, 0);
    if(m_fd == -1) {
        std::string err = strerror(errno);
        std::string ex = "waitable_event:" + err;
        throw vpgnixexception(ex.c_str());
    }
}

void waitable_event::set_event() {
    uint64_t u = 23;
    ssize_t s = read(m_fd, &u, sizeof(uint64_t));
}

void waitable_event::reset_event() {
    uint64_t u = 1;
    ssize_t s = write(m_fd, &u, sizeof(uint64_t));
}

waitable_event::~waitable_event() {
    if (m_fd != -1) {
        close(m_fd);
        m_fd = -1;
    }
}

int waitable_event::getId() { 
    return m_id; 
};

int waitable_event::getFd() { 
    return m_fd; 
};
