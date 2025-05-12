# Application objects to compile
objs := queue.o uthread.o preempt.o context.o #sem.o 

# Target library
lib := libuthread.a

all: $(lib)

# Avoid builtin rules and variables
MAKEFLAGS += -rR

# Don't print the commands unless explicitly requested with `make V=1`
ifneq ($(V),1)
Q = @
V = 0
endif

# Current directory
CUR_PWD := $(shell pwd)

# Define compilation toolchain
CC	= gcc

# General gcc options
CFLAGS	:= -Wall -Wextra -Werror
CFLAGS	+= -pipe
## Debug flag
ifneq ($(D),1)
CFLAGS	+= -O2
else
CFLAGS	+= -g
endif

# Dependency generation
CFLAGS	+= -MMD

# Include dependencies
deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

# Generic rule for building library
$(lib): $(objs)
	@echo "BUILDLIB	$@"
	$(Q)ar -rcs $@ $^

# Generic rule for compiling objects
%.o: %.c
	@echo "CC	$@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

# Cleaning rule
clean: FORCE
	@echo "CLEAN	$(CUR_PWD)"
	$(Q)rm -rf $(objs) $(deps) $(lib)

# Keep object files around
.PRECIOUS: %.o
.PHONY: FORCE
FORCE:

