objs := queue.o uthread.o preempt.o context.o sem.o 

lib := libuthread.a

all: $(lib)

MAKEFLAGS += -rR

ifneq ($(V),1)
Q = @
V = 0
endif

CUR_PWD := $(shell pwd)

CC	= gcc

CFLAGS	:= -Wall -Wextra -Werror
CFLAGS	+= -pipe

ifneq ($(D),1)
CFLAGS	+= -O2
else
CFLAGS	+= -g
endif

CFLAGS	+= -MMD

deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)

$(lib): $(objs)
	@echo "BUILDLIB	$@"
	$(Q)ar -rcs $@ $^

%.o: %.c
	@echo "CC	$@"
	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

clean: FORCE
	@echo "CLEAN	$(CUR_PWD)"
	$(Q)rm -rf $(objs) $(deps) $(lib)

.PRECIOUS: %.o
.PHONY: FORCE
FORCE:

