BUILDDIR := build/
OBJDIR := $(BUILDDIR)obj/
SRCDIR := src/
INCLUDEDIR := include/

SRC := $(shell find $(SRCDIR) -name "*.c")
OBJS := $(SRC:$(SRCDIR)%.c=$(OBJDIR)%.o)

TESTSRC := $(shell find tests -name "*.c")

LIB := build/dsalgo.a
TESTBIN := build/tests

.PHONY: run_tests
run_tests: $(TESTBIN)
	@echo "--- Running $(TESTBIN) ---"
	@$(TESTBIN)
	
$(TESTBIN): $(LIB) $(TESTSRC)
	$(CC) $(TESTSRC) $(LIB) -I $(INCLUDEDIR) -o $(TESTBIN)

$(LIB): $(OBJS)
	@mkdir -p $(dir $@)
	$(AR) -rcs $(LIB) $(OBJS)

$(OBJS): $(OBJDIR)%.o: $(SRCDIR)%.c
	@mkdir -p $(dir $@)
	$(CC) -c -I $(INCLUDEDIR) $< -o $@

.PHONY: bear
bear:
	bear -- make

