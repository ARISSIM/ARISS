CC 		:= g++

INCLUDE_DIR	:= sources/libApexArinc653/include
CC_FLAGS_ALL	:= -Wall -pedantic -g
LIBS		:= -lpthread -L$(shell pwd)/sources/libApexArinc653/dist/ -lApexArinc653

define SRC_2_OBJ
    $(foreach src,$(1),$(patsubst sources/%,build/%,$(src)))
endef

define SRC_2_BIN
    $(foreach src,$(1),$(patsubst sources/%,binary/%,$(src)))
endef

define SRC_2_SYM
    $(foreach src,$(1),$(patsubst sources/%.sym,binary/%,$(src)))
endef

all: lib targets 

build/%.o: sources/%.cpp
	@echo "  [CC]    $< -> $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CC_FLAGS_ALL) $(CC_FLAGS_SPEC) -I$(shell pwd)/sources/libApexArinc653/include -o $@ -c $< 

%.sym :
	for path in $^ ; do \
	    echo ln -fs $(shell pwd)/$$path $(call SRC_2_SYM, $@)/; \
	    ln -fs $(shell pwd)/$$path $(call SRC_2_SYM, $@)/; \
	done

binary/%.out:
	@echo "  [CC]    $< -> $@"
	@mkdir -p $(dir $@)
	@echo -I$(shell pwd)/sources/libApexArinc653/include  -o $@ $^ $(LIBS)
	@$(CC) -I$(shell pwd)/sources/libApexArinc653/include  -o $@ $^ $(LIBS)

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
	@(cd $(shell pwd)/sources/libApexArinc653/ && $(MAKE) $@)

symlinks: $(SYMLINKS)
	@echo symlinks created

info:
	@echo Targets [$(TARGETS)]
	@echo Objects [$(OBJECTS)]
	@echo Symlinks [$(SYMLINKS)]
	
link :
	@export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(shell pwd)/sources/lib/dist

	
lib:
	@(cd $(shell pwd)/sources/libApexArinc653/ && $(MAKE))
