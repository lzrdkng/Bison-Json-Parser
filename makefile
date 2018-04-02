#Compiler and Linker
CC          := gcc

#The Target Binary Program
TARGET      := libJSON.so
TARGETDIR   := lib

ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif


#The Directories, Source, Includes, Objects, Binary and Resources
BUILDDIR    := obj
DOCDIR      := doc
INCDIR      := inc
RESDIR      := res
SRCDIR      := src
TESTSDIR    := tests

DEPEXT      := d
OBJEXT      := o
SRCEXT      := c

#Flags, Libraries and Includes
CFLAGS      := -fPIC -Wall -Werror -pg -g -O0 -no-pie
INC         := -I$(INCDIR)
INCDEP      := -I$(INCDIR)
LIB         := -pg

# Bison source, header and yac files
BCC        := $(SRCDIR)/parser.c
BHH        := $(INCDIR)/parser.h
BYY        := $(INCDIR)/parser.y

# Bison flags
BFLAGS      := --defines=$(BHH) --output $(BCC)


#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell echo $(SRCDIR)/*.$(SRCEXT)) $(BCC)
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Default Make
all: $(BCC) directories $(TARGET)

# install
install:
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include/JSON
	install -m 557 $(TARGETDIR)/$(TARGET) $(PREFIX)/lib/
	install -m 557 $(INCDIR)/*.h $(PREFIX)/include/JSON


#Remake
remake: cleaner all

# Bison compilation
$(BCC): $(BYY)
	bison $(BFLAGS) $(BYY)

directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -shared -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp


# Cleaners

#Clean only Objects
clean:
	@$(RM) -rf $(BUILDDIR)

# Full Clean, Objects, Lib, Documentation, Bison, Tests
cleaner: clean clean-tests clean-doc clean-bison
	@$(RM) -rf $(TARGETDIR)


clean-bison:
	@$(RM) $(BHH) $(BCC)


clean-doc:
	@$(RM) -rf $(DOCDIR)

clean-tests:
	make -C $(TESTSDIR) cleaner

# Testing
tests:
	make -C $(TESTSDIR)

# Documentation
doc:
	doxygen doc.config


#Non-File Targets
.PHONY: all remake clean cleaner resources tests clean-tests doc clean-doc clean-bison
