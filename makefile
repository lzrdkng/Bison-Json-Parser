#Compiler and Linker
CC          := gcc

#The Target Binary Program
TARGET      := json

#The Directories, Source, Includes, Objects, Binary and Resources
TARGETDIR   := bin
SRCDIR      := src
INCDIR      := inc
BUILDDIR    := obj
RESDIR      := res
TESTSDIR    := tests
DOCDIR      := doc
SRCEXT      := c
DEPEXT      := d
OBJEXT      := o

#Flags, Libraries and Includes
CFLAGS      := -fPIC -Wall -ggdb -O0
LIB         :=
INC         := -I$(INCDIR)
INCDEP      := -I$(INCDIR)

# Bison flags and source
BYY        := $(INCDIR)/parser.y
BCC        := $(SRCDIR)/parser.c
BHH        := $(INCDIR)/parser.h
BFLAGS      := --defines=$(BHH) --output $(BCC)

#---------------------------------------------------------------------------------
#DO NOT EDIT BELOW THIS LINE
#---------------------------------------------------------------------------------
SOURCES     := $(shell echo $(SRCDIR)/*.$(SRCEXT))
OBJECTS     := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.$(OBJEXT)))

#Default Make
all: directories $(TARGET)

bison:
	bison $(BFLAGS) $(BYY)

#Remake
remake: cleaner all

directories:
	@mkdir -p $(TARGETDIR)
	@mkdir -p $(BUILDDIR)

#Clean only Objects
clean:
	@$(RM) -rf $(BUILDDIR)


clean-bison:
	@$(RM) $(BHH) $(BCC)

#Full Clean, Objects and Binaries
cleaner: clean clean-tests clean-doc clean-bison
	@$(RM) -rf $(TARGETDIR)

#Pull in dependency info for *existing* .o files
-include $(OBJECTS:.$(OBJEXT)=.$(DEPEXT))

#Link
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGETDIR)/$(TARGET) $^ $(LIB)

#Compile
$(BUILDDIR)/%.$(OBJEXT): $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
	@$(CC) $(CFLAGS) $(INCDEP) -MM $(SRCDIR)/$*.$(SRCEXT) > $(BUILDDIR)/$*.$(DEPEXT)
	@cp -f $(BUILDDIR)/$*.$(DEPEXT) $(BUILDDIR)/$*.$(DEPEXT).tmp
	@sed -e 's|.*:|$(BUILDDIR)/$*.$(OBJEXT):|' < $(BUILDDIR)/$*.$(DEPEXT).tmp > $(BUILDDIR)/$*.$(DEPEXT)
	@sed -e 's/.*://' -e 's/\\$$//' < $(BUILDDIR)/$*.$(DEPEXT).tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(BUILDDIR)/$*.$(DEPEXT)
	@rm -f $(BUILDDIR)/$*.$(DEPEXT).tmp

#Non-File Targets
.PHONY: all remake clean cleaner resources tests clean-tests doc clean-doc clean-bison

# Documentation
doc:
	doxygen doc.config

clean-doc:
	@$(RM) -rf $(DOCDIR)
