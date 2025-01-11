# USAGE:
#   build library:
#     make 
#
#   gcov repot
#     make -s gcov_report
#
#   build and run tests:
#     make -s test
#
#   build and run tests with sanitizers:
#     make -s test SAN=1
#
#   build with -g flag:
#     make -s test DEBUG=1
#
#   build selected test
#       make -s test CHK=div
#
#   build selected tests
#       make -s test CHK="compare div"
#
#   to add new tests just plase .check file in tests directory

CC = gcc
AR = ar

RM = rm -rf

ARFLAGS = rcs
CFLAGS = -Wall -Wextra -Werror -std=c11 

CHK = 

CSRS := matrix.c 

TARGET_LIB := matrix.a

BUILD_DIR = build

ifeq ($(DEBUG),1)
CFLAGS += -g
endif

ifeq ($(SAN),1)
CFLAGS += -g -fsanitize=undefined,address
LDFLAGS += -fsanitize=undefined,address
endif

COBJ := $(addprefix $(BUILD_DIR)/,$(CSRS:.c=.o))
DEPS := $(COBJ:.o=.d)

NODEPS := clean
.PHONY: all clean

all: $(TARGET_LIB)

gcov_report: add_coverage test
	@mkdir -p report
	gcovr --root . --exclude 'tests/.*' -o report/gcov_report.html --html-details --html-self-contained

add_coverage:
	$(eval CFLAGS += --coverage)
	$(eval LDFLAGS += --coverage)

$(TARGET_LIB): $(COBJ)
	$(AR) $(ARFLAGS) $@ $^

test: $(TARGET_LIB) 
	@$(MAKE) -C tests \
		CHK="$(CHK)" \
		CC=$(CC) \
		CFLAGS="$(CFLAGS)" \
		LDFLAGS="$(LDFLAGS) -L../" \
		LDLIBS="-l:$(TARGET_LIB)" \
		run 

$(COBJ): $(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(DEPS): $(BUILD_DIR)/%.d: %.c
	@mkdir -p $(dir $@)
	$(CC) -E $(CPPFLAGS) $< -MM -MT $(@:.d=.o) > $@

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NODEPS))))
include $(DEPS)
endif

clean:
	$(RM) $(COBJ) $(DEPS) $(TARGET_LIB) 
	$(RM) $(BUILD_DIR)
	$(RM) report
	@$(MAKE) -C ./tests clean

