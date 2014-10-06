##################################################
# ARINC653-simulator libApexArinc653Jni Makefile #
##################################################

CC		:= g++

CFLAGS= -fPIC -Wall #-fPIC -W -Wall -ansi -pedantic
LDFLAGS=
INCLUDE_DIR	:= -I../../include/libApexArinc653 -I../../include/libApexArinc653Jni -I/usr/lib/jvm/java-8-oracle/include -I/usr/lib/jvm/java-8-oracle/include/linux
LIBS		:= -L../../lib/ -lApexArinc653
BUILD_DIR	:=	

EXEC=libApexArinc653Jni.so
SRC= $(wildcard *.c)
OBJ= $(SRC:%.c=../../build/libApexArinc653Jni/%.o)

all: lib
	
lib: $(EXEC)

libApexArinc653Jni.so: $(OBJ)
	@echo "  [CC]    $< -> $@"
	@mkdir -p ../../lib
	@$(CC) $(CFLAGS) -shared -o ../../lib/$@ $^ $(LDFLAGS) $(INCLUDE_DIR) $(LIBS)


../../build/libApexArinc653Jni/%.o: %.c
	@echo "  [CC]    $< -> $@"
	@mkdir -p ../../build/libApexArinc653Jni
	@$(CC) $(CFLAGS) -o $@ -c $< $(INCLUDE_DIR)


clean:
	@rm -rf ../../lib/$(EXEC) $(OBJ)

mrproper: clean
	@rm -rf $(EXEC)


info : 
	@echo Exec [$(EXEC)]
	@echo Objects [$(OBJ)]