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

all: targets

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
	#@echo  $(LIBS) -I$(shell pwd)/sources/libApexArinc653/include -o $@ $^
	@$(CC)  $(LIBS) -I$(shell pwd)/sources/libApexArinc653/include  -o $@ $^

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
	@export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(shell pwd)/sources/lib/dist

	
lib:
	@gcc -c -fPIC -Wall -o $(shell pwd)/sources/libApexArinc653/dist/CQueuing.o $(shell pwd)/sources/libApexArinc653/src/CQueuing.c -I$(INCLUDE_DIR)
	@gcc -c -fPIC -Wall -o $(shell pwd)/sources/libApexArinc653/dist/CSampling.o $(shell pwd)/sources/libApexArinc653/src/CSampling.c -I$(INCLUDE_DIR)
	@gcc -c -fPIC -Wall -o $(shell pwd)/sources/libApexArinc653/dist/CBasefunction.o $(shell pwd)/sources/libApexArinc653/src/CBasefunction.c -I$(INCLUDE_DIR)
	@gcc -c -fPIC -Wall -o $(shell pwd)/sources/libApexArinc653/dist/Vector.o $(shell pwd)/sources/libApexArinc653/src/Vector.c -I$(INCLUDE_DIR)
	@gcc -fPIC -shared -o -Wl,-soname,libApexArinc653.so $(shell pwd)/sources/libApexArinc653/dist/libApexArinc653.so $(shell pwd)/sources/libApexArinc653/dist/CQueuing.o $(shell pwd)/sources/libApexArinc653/dist/CSampling.o $(shell pwd)/sources/libApexArinc653/dist/CBasefunction.o $(shell pwd)/sources/libApexArinc653/dist/Vector.o
	#gcc -L. -lmy main.o -o prog
