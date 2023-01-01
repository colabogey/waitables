#define DBUS_API_SUBJECT_TO_CHANGE
#include <dbus/dbus.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include <methodcaller.h>
#include <methodlistener.h>
#include <signalreceive.h>
#include <signalsend.h>

int main(int argc, char** argv)
{
   if (2 > argc) {
      printf ("Syntax: dbus-example [send|receive|listen|query] [<param>]\n");
      return 1;
   }
   methodcaller mc;
   methodlistener ml;
   signalreceive sr;
   signalsend ss;

   // zzzzz char* param = "no param";
   char paramval[] = "no param";
   char* param = paramval;
   if (3 >= argc && NULL != argv[2]) param = argv[2];
   if (0 == strcmp(argv[1], "send"))
      ss.sendsignal(param);
   else if (0 == strcmp(argv[1], "receive"))
      sr.receive();
   else if (0 == strcmp(argv[1], "listen"))
	  {
      	// ml.listen();
		ml.ml_thread_ptr =
			std::unique_ptr<std::thread>(new std::thread(&methodlistener::listen, &ml));
		printf("sleeping\n");
		sleep(20);
		ml.setstop(true);
		ml.ml_thread_ptr->join();
      }
   else if (0 == strcmp(argv[1], "query"))
   	  {
      	mc.query(param);
	  }
   else {
      printf ("Syntax: dbus-example [send|receive|listen|query] [<param>]\n");
      return 1;
   }
   return 0;
}
