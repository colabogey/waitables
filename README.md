# waitables
- Prototype aimed at providing linux versions of Windows WaitFor... objects.
- In windows there are HANDLEs for events, threads amd the like.
- An application can then WaitForMultipleObjects using these handles - like to get thread exit.
- Additionally, messages can be passed using PostThteadMessage and PostMessage to a message queue.
- These messages can also be the object of a WaitFor...
- And finally, timers can generate messages and therefore be the object of WaitFor...
## Everything is an fd
- The Linux versions of these facilities is based on the proposition that everything is an fd.
- Analagous facilities developed utilize eventfd, timerfd, signalfd.
- So a so-called waitablethread includes (hidden) an eventfd that can signal exit.
- Similarly for SetEvent, ResetEvent.
- Message posting utilizes standard message queues.
- There are other 'minor players', but this is the general idea.
## Now the WaitFor... is an epoll
- All of the analagous functions are represented by an fd.
- So WaitforMultipleObjects in Windows land becomes epoll for Linux.
- The hope is to make porting of windows apps somewhat easier.
