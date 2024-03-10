# Silly makefile in root folder because vscode makefile extension can't handle the makefile in subdir

SUBDIRS := src/

all: $(SUBDIRS)

$(SUBDIRS): FORCE
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

FORCE:

