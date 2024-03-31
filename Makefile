BUILDDIR := build/
OBJDIR := $(BUILDDIR)obj/
SRCDIR := src/

CFLAGS := -Wall -Wextra -g -I include/

SRC := $(shell find $(SRCDIR) -name "*.c")
OBJS := $(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)

TESTSRC := $(shell find tests -name "*.c")

LIB := build/dsalgo.a
TESTBIN := build/tests

.PHONY: run_tests
run_tests: $(TESTBIN)
	@echo "--- Running $(TESTBIN) ---"
	@valgrind $(TESTBIN)
	
$(TESTBIN): $(LIB) $(TESTSRC)
	$(CC) $(TESTSRC) $(LIB) $(CFLAGS) -lm -o $(TESTBIN)

$(LIB): $(OBJS)
	@mkdir -p $(dir $@)
	$(AR) -rcs $(LIB) $(OBJS)

$(OBJS): $(OBJDIR)%.o: $(SRCDIR)%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: bear
bear:
	bear -- make

