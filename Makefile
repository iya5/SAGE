UNAME = $(shell uname)

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g

INCLUDES = -Ilib/glfw/include -Ilib/glad/include -Ilib/cglm/include \
		   -Ilib/slog/include -Ilib/std -Ilib/nuklear/ -Ilib/stb

LDFLAGS = -lm lib/glad/src/gl.o lib/glfw/src/libglfw3.a \
		  lib/slog/src/slog.o

ifeq ($(UNAME), Linux)
	CFLAGS += -std=c23
	LDFLAGS += -lGL -lX11 -lpthread -lXrandr -lXi -ldl
endif

ifeq ($(UNAME), Darwin)
	CFLAGS += -v -std=c2x -Wgnu-zero-variadic-macro-arguments
	LDFLAGS += -framework Foundation -framework AppKit -framework CoreVideo \
			   -framework Cocoa -framework IOKit -framework Metal \
			   -framework QuartzCore
endif

CFLAGS += $(INCLUDES)


SRC = $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin

.PHONY: all
all: dirs libs sage

libs:
	cd lib/glfw && cmake . && make
	cd lib/glad && $(CC) -o src/gl.o -Iinclude -c src/gl.c
	cd lib/slog && $(CC) -o src/slog.o -Iinclude -c src/slog.c

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
