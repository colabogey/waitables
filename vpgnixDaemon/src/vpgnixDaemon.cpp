#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signalfd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>
#include <vpgnixApp.h>
#include <vpgnixDaemon.h>
#include <vpgnixlogger.h>

void makeDaemon();

int main(int argc, char **argv) {
    makeDaemon();
    vpgnixDaemon vpd;
    vpd.setupSignals();
    vpd.init();
    vpd.run();
    vpd.uninit();
    exit(EXIT_SUCCESS);
}

vpgnixDaemon::vpgnixDaemon() : m_keepRunning(false), m_sfd(-1), m_pid(-1) {}

void vpgnixDaemon::init() {
    std::shared_ptr<loggerConfig> lc = std::make_shared<loggerConfig>();
    m_logger = std::make_shared<vpgnixlogger>();
    m_logger->Initialize(lc);
    m_pid = getpid();
    m_logger->Log(LOGLEVEL_INFO, "daemon: ****** pid (%d) *****", m_pid);
    m_app = std::make_shared<vpgnixApp>(m_logger);
}

void vpgnixDaemon::uninit() { m_logger->Uninitialize(); }
void vpgnixDaemon::run() {
    ssize_t s;
    struct signalfd_siginfo siginfo;
    m_keepRunning = true;
    m_logger->Log(LOGLEVEL_INFO, "daemon: starting vpgnixApp");
    m_app->start();
    while (m_keepRunning) {
        s = read(m_sfd, &siginfo, sizeof(siginfo));
        if (s != sizeof(siginfo)) {
            m_logger->Log(LOGLEVEL_ERROR,
                            "daemon: read on m_sfd failed (%d) (%s) ", errno,
                            strerror(errno));
        }
        if (siginfo.ssi_signo == SIGHUP) {
            handleSIGHUP();
        } else if (siginfo.ssi_signo == SIGTERM) {
            handleSIGTERM();
        } else {
            m_logger->Log(LOGLEVEL_WARNING, "daemon: unexprcted signal (%d) ",
                            siginfo.ssi_signo);
        }
    }
    m_logger->Log(LOGLEVEL_INFO, "daemon: stopping vpgnixApp");
    m_app->stop();
    sleep(5);
    m_logger->Log(LOGLEVEL_INFO, "daemon: fini !!");
}

// SUGHUP is 1
void vpgnixDaemon::handleSIGHUP() {
    m_logger->Log(LOGLEVEL_INFO, "daemon: SIGHUP re-read configuration");
}

// SUGTERM is 15
void vpgnixDaemon::handleSIGTERM() {
    m_logger->Log(LOGLEVEL_INFO, "daemon: SIGTERM stopping");
    m_keepRunning = false;
}

void vpgnixDaemon::setupSignals() {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGHUP);
    sigaddset(&mask, SIGTERM);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        m_logger->Log(LOGLEVEL_ERROR, "daemon: sigprocmask failed (%d) (%s) ",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    m_sfd = signalfd(-1, &mask, 0);
    if (m_sfd == -1) {
        m_logger->Log(LOGLEVEL_ERROR, "daemon: signalfd failed (%d) (%s) ",
                        errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

// the preferred 'new style' daemon process generation

void makeDaemon() {
    // Fork off the parent process
    pid_t pid = fork();

    // An error occurred
    if (pid < 0) {
        fprintf(stderr, "daemon: fork 1 failed (%d) (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Success: Let the parent terminate
    if (pid > 0) {
        fprintf(stderr, "daemon: fork 1 parent exiting. Child pid (%d)\n", pid);
        exit(EXIT_SUCCESS);
    }

    // On success: The child process becomes session leader
    if (setsid() < 0) {
        fprintf(stderr, "daemon: fork 1 child setsid failed (%d) (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Fork off for the second time
    pid = fork();

    // An error occurred
    if (pid < 0) {
        fprintf(stderr, "daemon: fork 2 failed (%d) (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Success: Let the parent terminate
    if (pid > 0) {
        fprintf(stderr, "daemon: fork 2 parent exiting. Child pid (%d)\n", pid);
        exit(EXIT_SUCCESS);
    }

    // signals
    // done external to this routine

    // Set new file permissions
    umask(0);

    // Change the working directory to the root directory
    // or another appropriated directory
    chdir("/");

    // Close all open file descriptors
    // skip this for inital debug - log messaeges to screen
    // for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
    //    close(x);
    //}
    return;
}
