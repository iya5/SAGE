UNAME = $(shell uname)

VERSION = 0.0.0

CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -g -Wno-variadic-macros \
		 -DSAGE_VERSION=\"$(VERSION)\"

INCLUDES = -Ilib/glfw/include -Ilib/glad/include \
		   -Ilib/std -Ilib/nuklear/ -Ilib/stb

LDFLAGS = -lm lib/glad/src/gl.o lib/glfw/src/libglfw3.a

# add compile flag for defining SAGE_RELEASE as 1

ifeq ($(UNAME), Linux)
	CFLAGS += -std=c99
	LDFLAGS += -lGL -lX11 -lpthread -lXrandr -lXi -ldl
endif

ifeq ($(UNAME), Darwin)
	CFLAGS += -v -std=c99 -Wgnu-zero-variadic-macro-arguments
	LDFLAGS += -framework Foundation -framework AppKit -framework CoreVideo \
			   -framework Cocoa -framework IOKit -framework Metal \
			   -framework QuartzCore
endif

CFLAGS += $(INCLUDES)

SRC = $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin

all: lib sage

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

sage: $(OBJ)
	mkdir -p $(BIN)
	$(CC) -o $(BIN)/sage $^ $(LDFLAGS)

lib:
	cd lib/glfw && cmake . && make
	cd lib/glad && $(CC) -o src/gl.o -Iinclude -c src/gl.c

run:
	$(BIN)/sage

clean:
	rm -rf $(BIN) $(OBJ)

.PHONY: all sage lib run clean
