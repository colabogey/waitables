# waitables
- Prototype aimed at providing linux versions of Windows WaitFor... objects.
- In windows there are HANDLEs for events, threads amd the like.
- An application can then WaitForMultipleObjects using these handles - like to get thread exit.
- Additionally, messages can be passed using PostThreadMessage and PostMessage to a message queue.
- These messages can also be the object of a WaitFor... via GetMessage from a message queue.
- And finally, timers can generate messages and therefore be the object of WaitFor...
## Everything is an fd
- The Linux versions of these facilities is based on the proposition that everything is an fd.
- Analagous facilities developed utilize eventfd, timerfd, signalfd.
- So a so-called waitablethread, for example, includes (hidden) an eventfd that can signal exit.
- Similarly for SetEvent, ResetEvent.
- Message posting utilizes standard message queues.
- There are other 'minor players', but this is the general idea.
## Now the WaitFor... is an epoll
- All of the analagous functions are represented by an fd.
- So WaitforMultipleObjects in Windows land becomes epoll for Linux.
- The hope is to make porting of windows apps somewhat easier.
# Download, Build, Run
## Download
- Clone the repo
  - I put it in ${HOME}/projects
  - The makefiles navigate like this
```
PROJECT_NAME = waitables
ifeq (, $(PROJECTS_BASE))
PROJECT_BASE = ${HOME}/projects/${PROJECT_NAME}
else
PROJECT_BASE = ${PROJECTS_BASE}/${PROJECT_NAME}
endif
```
  - So presumably you can locate the project base anywhere and set the environment variable PROJECTS_BASE appropriately. I think I tried this once. Maybe just put it in ${HOME}/projects.
## Build
- cd waitables
- make
  - there are several make targets, but this will build the release version. And that's fine to start with.
## Run
- cd waitables/bin
- sudo rm /tmp/vpgnixDaemon_0.log
  - this is the default logname. Configurability has not been added yet.
- sudo ./vpgnixDaemon
  - message queues are used for the get/post message and these require elevated priveledge
  - eventually this will run under the auspices of init iand/or systemctl
  - take note of the PID
- tail -f /tmp/vpgnixDaemon_0.log
- the daemon responds to a couple of signals
  - SIGHUP (sudo kill -1 pid) provikes a configuration reload
    - not implemented - just a log message
  - SIGTERM (sudo kill -15 pid) will initiate an orderly stop
- Sooooooooooooo
  - fire it up
  - watch some log messages
  - send a couple of SIGHUP's just for fun
  - when the log say that timers and message generation is done, send the SIGTERM
### What does it do ?
- the daemon simply exercises the various waitables
## Code Quality
- this is prototype code
  - in that range this would fall at the proof of concept end of the spectrum
  - other prototype features can/will be added
    - re-config from an actual file change, for example
# Platform Info
- most work has been done on Ubuntu
```lsb_release -a
Distributor ID: Ubuntu
Description:    Ubuntu 22.04.2 LTS
Release:        22.04
Codename:       jammy
```
- I also make sure, from time to time it works on OpenSUSE
```
lsb_release -a
LSB Version:    n/a
Distributor ID: openSUSE
Description:    openSUSE Tumbleweed
Release:        20230304
Codename:       n/a
```
- you can probably use either.
