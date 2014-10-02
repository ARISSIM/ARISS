CC		:= gcc

CFLAGS= -fPIC -Wall #-fPIC -W -Wall -ansi -pedantic
LDFLAGS=
INCLUDE_DIR	:= include

EXEC=libApexArinc653.so
SRC= $(wildcard src/*.c)
OBJ= $(SRC:src/%.c=build/%.o)

all: $(EXEC)

libApexArinc653.so: $(OBJ)
	@echo "  [CC]    $< -> $@"
	@$(CC) $(CFLAGS) -shared -o dist/$@ $^ $(LDFLAGS) -I$(INCLUDE_DIR)


build/%.o: src/%.c
	@echo "  [CC]    $< -> $@"
	@echo  $@ -c $< $(CFLAGS) -I$(INCLUDE_DIR)
	@mkdir -p $(dir $@)
	@$(CC) -o $@ -c $< $(CFLAGS) -I$(INCLUDE_DIR)	


clean:
	@rm -rf dist/$(EXEC) $(OBJ)

mrproper: clean
	@rm -rf $(EXEC)


info : 
	@echo Exec [$(EXEC)]
	@echo Objects [$(OBJ)]


