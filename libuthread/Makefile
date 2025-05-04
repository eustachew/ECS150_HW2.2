# Target library
lib := libuthread.a

# Source files
src := context.c preempt.c queue.c sem.c uthread.c

# Object files
obj := $(src:.c=.o)

# Compiler and flags
CC := gcc
CFLAGS := -Wall -Werror -g

all: $(lib)

# Build the static library
$(lib): $(obj)
	ar rcs $@ $^

# Clean up object files and library
clean:
	rm -f $(obj) $(lib)
	
## TODO: Phase 1
