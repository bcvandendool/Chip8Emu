TARGET   = emulator

CC       = gcc
CFLAGS   = -std=c99 -Wall -Wextra -I.

LINKER   = gcc
LFLAGS   = -Wall -I. -lm -lSDL2

DEPDIR := .deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPENDS  := $(SOURCES:$(SRCDIR)/%.c=$(DEPDIR)/%.d)

rm       = rm -f

all: $(BINDIR)/$(TARGET)
.PHONY: all

$(BINDIR)/$(TARGET): $(OBJECTS)
	@echo "LINK $@"
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c Makefile $(DEPDIR)
	@echo "CC $<"
	@$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(DEPDIR): ; @mkdir -p $@

-include $(DEPENDS)

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@$(rm) $(BINDIR)/$(TARGET)
	@$(rm) $(DEPENDS)
	@rm -rf $(DEPDIR)

.PHONY: run
run: $(BINDIR)/$(TARGET)
	@./$(BINDIR)/$(TARGET)