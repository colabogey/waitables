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
LIBFLAGS = -lnetwork -lvpgnixexception -lgtest -lgmock -L$(PROJECT_BIN_DIR) -L/usr/local/lib -L/usr/local/lib64

SRC_DIR     = ./
BUILD_DIR   = ./build
BIN_DIR     = ./bin
NETWORK_LIB     = ../src/bin/libnetwork.a
MYTESTAPP = networktest

$(shell mkdir -p $(PROJECT_BIN_DIR))
$(shell mkdir -p $(BIN_DIR))
$(shell mkdir -p $(BUILD_DIR))

include includes.mk

SRCS    = $(wildcard $(SRC_DIR)/*.cpp)
OBJS    = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.cpp=.o)))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

.PHONY: all
all:    $(BIN_DIR)/$(MYTESTAPP)

$(WAITABLE_LIB):
	touch $(SRCS)

$(BIN_DIR)/$(MYTESTAPP):	$(OBJS) $(WAITABLE_LIB)
	$(CC) $(LDFLAGS) -o $(BIN_DIR)/$(MYTESTAPP) $(OBJS) $(LIBFLAGS) -lpthread -lc -lgcov

.PHONY: coverage
coverage:
	$(BIN_DIR)/$(MYTESTAPP)
	lcov -c --directory ../src/build --output-file coverage.info
	genhtml coverage.info --output-directory coverage_out

.PHONY: tests
tests:
	$(BIN_DIR)/$(MYTESTAPP)

.PHONY: testclean
testclean:
	rm -f $(BUILD_DIR)/*.gcda $(BUILD_DIR)/*.gcno $(BIN_DIR)/$(MYTESTAPP)
	rm -rf coverage_out
	rm coverage.info

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/* $(BIN_DIR)/*
	rm -rf coverage_out
	rm -f coverage.info

.PHONY:	format
format:
	clang-format -i -style=file $(SRCS)
	clang-format -i -style="{IndentWidth: 4}" $(SRCS)

