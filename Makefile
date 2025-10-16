UNAME = $(shell uname)

CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -Wpedantic -g -std=c23
CXXFLAGS = -Wall -Wextra -Wpedantic -std=c++17

CFLAGS += -Ilib/glfw/include -Ilib/glad/include -Ilib/cglm/include
CFLAGS += -Ilib/slog/include -Ilib/openal-soft/include -Ilib/stb
CFLAGS += -Ilib/imgui -Ilib/imgui/backends -Ilib/dear_bindings
CFLAGS += -Ilib/dear_bindings/generated

LDFLAGS = lib/glad/src/glad.o lib/glfw/src/libglfw3.a -lm
LDFLAGS += lib/slog/src/slog.o lib/openal-soft/build/libopenal.so

ifeq ($(UNAME), Linux)
	LDFLAGS += -lGL -lX11 -lpthread -lXrandr -lXi -ldl
endif

SRC = $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJ = $(SRC:.c=.o)
BIN = bin

# imgui compilation
IMGUI_CORE = imgui.cpp imgui_draw.cpp imgui_tables.cpp imgui_widgets.cpp
IMGUI_CORE += imgui_demo.cpp
IMGUI_BACKENDS = backends/imgui_impl_opengl3.cpp backends/imgui_impl_glfw.cpp

# imgui bindings for c
DIMGUI_CORE = dcimgui.cpp dcimgui_internal.cpp
DIMGUI_BACKENDS = generated/dcimgui_impl_glfw.cpp 
DIMGUI_BACKENDS += generated/dcimgui_impl_opengl3.cpp

.PHONY: all
all: dirs libs sage

libs:
	cd lib/glfw && cmake . && make
	cd lib/glad && $(CC) -o src/glad.o -Iinclude -c src/glad.c
	cd lib/openal-soft/build && cmake .. && cmake --build .
	cd lib/slog && $(CC) -o src/slog.o -Iinclude -c src/slog.c
	# compiling imgui
	cd lib/dear_bindings && chmod +x BuildAllBindings.sh && \
		python -v venv venv && source venv/bin/activate && \
		pip install -r requirements.txt && ./BuildAllBindings.sh
	cd lib/imgui && g++ -c

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
