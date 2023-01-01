#!/usr/bin/make -f

PROJECT_NAME = waitables
ifeq (, $(PROJECT_BASE))
PROJECTS_BASE := ${HOME}/projects
PROJECT_BASE := ${PROJECTS_BASE}/${PROJECT_NAME}
endif

include $(PROJECT_BASE)/toplevel.mk

CFLAGS = $(CFLAGSTOUSE) \
	-Wall -Wno-unknown-pragmas -Wno-sign-compare \
	-Woverloaded-virtual -Wwrite-strings -Wno-unused \
	$(INCDIRS) $(DEFS)

LDFLAGS = 
#
LIBFLAGS = -lvpgnixApp -lcstring -lwaitable -lvpgnixlogger -lvpgnixexception \
            -lgtest -lgmock -lpthread -lc -lgcov -lrt \
	        -L$(PROJECT_BIN_DIR)

SRC_DIR     = ./
BUILD_DIR   = ./build
BIN_DIR     = ./bin
INC_DIR	    = ../include

include includes.mk

VPGNIXAP_LIB	= $(PROJECT_BIN_DIR)/libvpgnixApp.a
WAITABLE_LIB	= $(PROJECT_BIN_DIR)/libwaitable.a
LOGGER_LIB  	= $(PROJECT_BIN_DIR)/libvpgnixlogger.a
EXCEPTION_LIB	= $(PROJECT_BIN_DIR)/libvpgnixexception.a
LIBDEPS = $(VPGNIX_LIB) $(WAITABLE_LIB) $(LOGGER_LIB) $(EXCEPTION_LIB)

SRCS    = $(wildcard $(SRC_DIR)/*.cpp)
OBJS    = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.cpp=.o)))
IDEPS    = $(wildcard $(INC_DIR)/*.h)
MYBINNAME	= vpgnixDaemon

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(IDEPS)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

.PHONY: all
all: $(BIN_DIR)/$(MYBINNAME)

$(VPGNIXAP_LIB):
	touch $(SRCS)

$(WAITABLE_LIB):
	touch $(SRCS)

$(EXCEPTION_LIB):
	touch $(SRCS)

$(LOGGER_LIB):
	touch $(SRCS)

$(BIN_DIR)/$(MYBINNAME):	$(OBJS) $(LIBDEPS)
	$(CC) $(LDFLAGS) -o $(BIN_DIR)/$(MYBINNAME) $(OBJS) $(LIBFLAGS)

.PHONY: coverage
coverage:
	echo "nothing to be done for coverage"

.PHONY: test
test:
	echo "nothing to be done for test"

.PHONY: testclean
testclean:
	echo "nothing to be done for testclean"

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/* $(BIN_DIR)/*

.PHONY: format
format:
	clang-format -i -style=file $(SRCS)
	clang-format -i -style="{IndentWidth: 4}" $(SRCS)
	clang-format -i -style=file $(IDEPS)
	clang-format -i -style="{IndentWidth: 4}" $(IDEPS)

