###############################################
# ARINC653-simulator libApexArinc653 Makefile #
###############################################

CC		:= g++

CFLAGS= -fPIC -Wall #-fPIC -W -Wall -ansi -pedantic
LDFLAGS=
INCLUDE_DIR	:= ../../include/libApexArinc653
BUILD_DIR	:=	

EXEC=libApexArinc653.so
SRC= $(wildcard *.c)
OBJ= $(SRC:%.c=../../build/libApexArinc653/%.o)

all: lib
	
lib : $(EXEC)

libApexArinc653.so: $(OBJ)
	@echo "  [CC]    $< -> $@"
	@mkdir -p ../../lib
	@$(CC) $(CFLAGS) -shared -o ../../lib/$@ $^ $(LDFLAGS) -I$(INCLUDE_DIR)


../../build/libApexArinc653/%.o: %.c
	@pwd
	@echo "  [CC]    $< -> $@"
	@echo  $@ -c $< $(CFLAGS) -I$(INCLUDE_DIR)
	@mkdir -p ../../build/libApexArinc653
	@$(CC) -o $@ -c $< $(CFLAGS) ../../build/libApexArinc653 -I$(INCLUDE_DIR) 	


clean:
	@rm -rf ../../lib/$(EXEC) $(OBJ)

mrproper: clean
	@rm -rf $(EXEC)


info : 
	@echo Exec [$(EXEC)]
	@echo Objects [$(OBJ)]


