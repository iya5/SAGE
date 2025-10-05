UNAME = $(shell uname)

CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g -std=c23

CFLAGS += -Ilib/glfw/include -Ilib/glad/include -Ilib/cglm/include
CFLAGS += -Ilib/slog/include -Ilib/openal-soft/include

LDFLAGS = lib/glad/src/glad.o lib/glfw/src/libglfw3.a -lm
LDFLAGS += lib/slog/src/slog.o lib/openal-soft/build/libopenal.so

ifeq ($(UNAME), Linux)
	LDFLAGS += -lGL -lX11 -lpthread -lXrandr -lXi -ldl
endif

SRC = $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin

.PHONY: all
all: dirs libs sage

libs:
	cd lib/glfw && cmake . && make
	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/openal-soft/build && cmake .. && cmake --build .
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
