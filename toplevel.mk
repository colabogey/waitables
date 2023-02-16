#
# global definitions
PROJECT_NAME = waitables
ifeq (, $(PROJECTS_BASE))
PROJECT_BASE = ${HOME}/projects/${PROJECT_NAME}
else
PROJECT_BASE = ${PROJECTS_BASE}/${PROJECT_NAME}
endif

PROJECT_BIN_DIR = $(PROJECT_BASE)/bin
PROJECT_INCLUDE_DIR = $(PROJECT_BASE)/include

OBSERVER_INTERFACE_DIR = $(PROJECT_INCLUDE_DIR)/observer
LOGGER_INTERFACE_DIR = $(PROJECT_INCLUDE_DIR)/logger_interface
WAITABLE_INTERFACE_DIR = $(PROJECT_INCLUDE_DIR)/waitable
VPGNIXAPP_INTERFACE_DIR = $(PROJECT_INCLUDE_DIR)/vpgnixApp
EXCEPTION_INTERFACE_DIR = $(PROJECT_INCLUDE_DIR)/vpgnixexception
CODELIBS_INTERFACE_DIR = $(PROJECT_INCLUDE_DIR)/codelibs
CSTRING_INTERFACE_DIR = $(PROJECT_INCLUDE_DIR)/cstring

$(info PROJECT_BASE="$(PROJECT_BASE)")
$(info PROJECT_BIN_DIR="$(PROJECT_BIN_DIR)")
$(info PROJECT_INCLUDE_DIR="$(PROJECT_INCLUDE_DIR)")
$(info OBSERVER_INTERFACE_DIR="$(OBSERVER_INTERFACE_DIR)")
$(info LOGGER_INTERFACE_DIR="$(LOGGER_INTERFACE_DIR)")
$(info WAITABLE_INTERFACE_DIR="$(WAITABLE_INTERFACE_DIR)")
$(info VPGNIXAPP_INTERFACE_DIR="$(VPGNIXAPP_INTERFACE_DIR)")
$(info EXCEPTION_INTERFACE_DIR="$(EXCEPTION_INTERFACE_DIR)")
$(info CSTRING_INTERFACE_DIR="$(CSTRING_INTERFACE_DIR)")

##
#global (common) settings
SHELL = /bin/sh
CC = g++

DEFS =  -DLINUX=1
#CFLAGSDEBUG = -std=c++2a -O0 -g3 -fmessage-length=0 -MMD -MP -fprofile-arcs -ftest-coverage
#CFLAGSRELEASE = -std=c++2a -O0 -g3 -fmessage-length=0 -MMD -MP
CFLAGSDEBUG = -std=c++17 -O0 -g3 -fmessage-length=0 -MMD -MP -fprofile-arcs -ftest-coverage
CFLAGSRELEASE = -std=c++17 -O0 -g3 -fmessage-length=0 -MMD -MP

$(info DEBUG="$(DEBUG)")
ifeq (TRUE, $(DEBUG))
CFLAGSTOUSE = $(CFLAGSDEBUG)
else
CFLAGSTOUSE = $(CFLAGSRELEASE)
endif
$(info CFLAGSTOUSE="$(CFLAGSTOUSE)")

