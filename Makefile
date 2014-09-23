CC 		:= g++

INCLUDE_DIR	:= sources/include
CC_FLAGS_ALL	:= -Wall -pedantic -g
LIBS		:= -lpthread -L$(shell pwd)sources/common/lib/libCBasefunctions.so

define SRC_2_OBJ
    $(foreach src,$(1),$(patsubst sources/%,build/%,$(src)))
endef

define SRC_2_BIN
    $(foreach src,$(1),$(patsubst sources/%,binary/%,$(src)))
endef

define SRC_2_SYM
    $(foreach src,$(1),$(patsubst sources/%.sym,binary/%,$(src)))
endef

all: targets

build/%.o: sources/%.cpp
	@echo "  [CC]    $< -> $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS_ALL) $(CC_FLAGS_SPEC) -o $@ -c $<

%.sym :
	for path in $^ ; do \
	    #echo ln -fs $(shell pwd)/$$path $(call SRC_2_SYM, $@)/; \
	    ln -fs $(shell pwd)/$$path $(call SRC_2_SYM, $@)/; \
	done

binary/%.out:
	@echo "  [CC]    $< -> $@"
	@mkdir -p $(dir $@)
	@$(CC) -o $@ $^ $(LIBS)

# Overriden in rules.mk
TARGETS :=
OBJECTS :=
SYMLINKS :=

dir	:= sources
include	$(dir)/rules.mk

targets: link $(patsubst sources/%, binary/%, $(TARGETS)) symlinks

clean:
	@rm -f $(TARGETS) $(OBJECTS)

mrproper :
	@rm -f $(TARGETS) $(OBJECTS)
	@find -name *~ | xargs rm -f
	@find -name "*.fifo" | xargs rm -f

symlinks: $(SYMLINKS)
	@echo symlinks created

info:
	@echo Targets [$(TARGETS)]
	@echo Objects [$(OBJECTS)]
	@echo Symlinks [$(SYMLINKS)]
	
link :
	@export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(shell pwd)/sources/common/lib

	
lib:
	g++ -shared -fPIC -o $(shell pwd)/sources/common/lib/CBasefunctions.so ./sources/common/CBasefunction.cpp
	#gcc -L. -lmy main.o -o prog
