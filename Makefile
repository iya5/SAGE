CC = gcc
CFLAGS = -Wall -Wextra -g
CFLAGS += -Ilib/glfw/include -Ilib/glad/include -Ilib/cglm/include
LDFLAGS = lib/glad/src/gl.o lib/glfw/src/libglfw3.a -lm

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin

.PHONY: all
all: dirs libs sage

libs:
	cd lib/glfw && cmake . && make
	cd lib/glad && $(CC) -o src/gl.o -Iinclude -c src/gl.c

dirs: 
	mkdir -p ./$(BIN)

run:
	$(BIN)/sage

sage: $(OBJ)
	$(CC) -o $(BIN)/sage $^ $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(BIN) $(OBJ)
