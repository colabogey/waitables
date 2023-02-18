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
LIBFLAGS = -lwatcher -lgtest -lgmock -L$(PROJECT_BIN_DIR) -L/usr/local/lib -L/usr/local/lib64

SRC_DIR     = ./
BUILD_DIR   = ./build
BIN_DIR     = ./bin
WATCHER_LIB     = ../src/bin/libwatcher.a

$(shell mkdir -p $(PROJECT_BIN_DIR))
$(shell mkdir -p $(BIN_DIR))
$(shell mkdir -p $(BUILD_DIR))

include includes.mk

SRCS    = $(wildcard $(SRC_DIR)/*.cpp)
OBJS    = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.cpp=.o)))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

.PHONY: all
all:    $(BIN_DIR)/test_watcher

$(WATCHER_LIB):
	touch $(SRCS)

$(BIN_DIR)/test_watcher:	$(OBJS) $(WATCHER_LIB)
	$(CC) $(LDFLAGS) -o $(BIN_DIR)/test_watcher $(OBJS) $(LIBFLAGS) -lpthread -lc -lgcov

.PHONY: install
install: all
	$(INSTALL) tar $(bindir)/$(binprefix)tar
	-test ! -f rmt || $(INSTALL) rmt /etc/rmt
	$(INSTALLDATA) $(SRCDIR)/tar.info* $(infodir)

TAGS:   $(SRCS)
	etags $(SRCS)

.PHONY: coverage
coverage:
	$(BIN_DIR)/test_watcher
	lcov -c --directory ../src/build --output-file coverage.info
	genhtml coverage.info --output-directory coverage_out

.PHONY: test
test:
	$(BIN_DIR)/test_watcher

.PHONY: testclean
testclean:
	rm -f $(BUILD_DIR)/*.gcda $(BUILD_DIR)/*.gcno $(BIN_DIR)/test_watcher coverage.info
	rm -rf coverage_out

.PHONY: clean
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d $(BUILD_DIR)/*.gcda $(BUILD_DIR)/*.gcno $(BIN_DIR)/test_watcher coverage.info
	rm -rf coverage_out

