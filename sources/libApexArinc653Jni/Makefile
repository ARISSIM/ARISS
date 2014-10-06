CC		:= g++

CFLAGS= -fPIC -Wall #-fPIC -W -Wall -ansi -pedantic
LDFLAGS=
INCLUDE_DIR	:= ../../include/libApexArinc653Jni
BUILD_DIR	:=	

EXEC=libApexArinc653Jni.so
SRC= $(wildcard *.c)
OBJ= $(SRC:%.c=../../build/libApexArinc653Jni/%.o)

all: $(EXEC)

libApexArinc653Jni.so: $(OBJ)
	@echo "  [CC]    $< -> $@"
	@$(CC) $(CFLAGS) -shared -o ../../lib/$@ $^ $(LDFLAGS) -I$(INCLUDE_DIR)


../../build/libApexArinc653Jni/%.o: %.c
	@echo "  [CC]    $< -> $@"
	@echo  $@ -c $< $(CFLAGS) -I$(INCLUDE_DIR)
#	@mkdir -p $(dir $@)
	@$(CC) -o $@ -c $< $(CFLAGS) ../../build/libApexArinc653Jni -I$(INCLUDE_DIR) 	


clean:
	@rm -rf ../../lib/$(EXEC) $(OBJ)

mrproper: clean
	@rm -rf $(EXEC)


info : 
	@echo Exec [$(EXEC)]
	@echo Objects [$(OBJ)]


