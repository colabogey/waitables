SUBDIRS := logger_interface vpgnixexception cstring logger inotify \
    observer waitable vpgnixApp vpgnixDaemon

PROJECT_NAME = waitables
ifeq (, $(PROJECTS_BASE))
PROJECT_BASE = ${HOME}/projects/${PROJECT_NAME}
else
PROJECT_BASE = ${PROJECTS_BASE}/${PROJECT_NAME}
endif

include $(PROJECT_BASE)/generic.mk
