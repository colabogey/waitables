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
LIBFLAGS = -lgtest -lgmock -L../../bin

SRC_DIR     = ./
BUILD_DIR   = ./build
BIN_DIR     = ./bin
INC_DIR     = ../include

$(shell mkdir -p $(PROJECT_BIN_DIR))
$(shell mkdir -p $(BIN_DIR))
$(shell mkdir -p $(BUILD_DIR))

include includes.mk

SRCS    = $(wildcard $(SRC_DIR)/*.cpp)
OBJS    = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.cpp=.o)))
IDEPS    = $(wildcard $(INC_DIR)/*.h)

.PHONY: release
release:
	mkdir -p $(OBSERVER_INTERFACE_DIR)
	cp $(INC_DIR)/*.h $(OBSERVER_INTERFACE_DIR)

.PHONY: debug
debug:
	mkdir -p $(OBSERVER_INTERFACE_DIR)
	cp $(INC_DIR)/*.h $(OBSERVER_INTERFACE_DIR)

.PHONY: tests
tests:
	@echo "nothing to do for test"

.PHONY: coverage
coverage:
	@echo "nothing to do for coverage"

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d \
	$(BUILD_DIR)/*.gcda $(BUILD_DIR)/*.gcno $(BIN_DIR)/watcher coverage.info
	rm -rf coverage_out

.PHONY: testclean
testclean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d \
	$(BUILD_DIR)/*.gcda $(BUILD_DIR)/*.gcno $(BIN_DIR)/watcher coverage.info
	rm -rf coverage_out

