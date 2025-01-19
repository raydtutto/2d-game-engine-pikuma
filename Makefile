# Pikuma course Makefile | START -----------------------------------------------------

#build:
#	g++ -std=c++17 -Wall $(sdl2-config --cflags) src/*.cpp -o gameengine $(sdl2-config --libs)
#
#run:
#	./gameengine
#
#clean:
#	rm gameengine

# Pikuma course Makefile | END -----------------------------------------------------

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall

# SDL2 flags (adjust if SDL2 is installed in a non-default location)
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LDFLAGS = $(shell sdl2-config --libs)

# Output binary name
TARGET = gameengine

# Source files
SRC = src/Main.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Linking the final executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(SDL2_LDFLAGS)

# Compiling source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SDL2_CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJ) $(TARGET)

run:
	./$(TARGET)

# Phony targets
.PHONY: all clean