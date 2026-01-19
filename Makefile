# Day/Night Tower Defense - Cross-platform Makefile
# Works on both Windows (MinGW) and Linux

# Detect OS
ifeq ($(OS),Windows_NT)
    PLATFORM = WINDOWS
    EXE = .exe
    RAYLIB_PATH = raylib
    SHELL = cmd
else
    PLATFORM = LINUX
    EXE =
    RAYLIB_PATH = /usr/local
endif

# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
INCLUDES = -Iinclude -I$(RAYLIB_PATH)/include
LDFLAGS = -L$(RAYLIB_PATH)/lib

# Libraries
ifeq ($(PLATFORM),WINDOWS)
    LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm
else
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Target executable
TARGET = $(BIN_DIR)/tower_defense$(EXE)

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS) $(LIBS)
	@echo Build complete: $(TARGET)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create directories
$(BUILD_DIR):
	@mkdir $(BUILD_DIR)

$(BIN_DIR):
	@mkdir $(BIN_DIR)

# Run the game
run: all
ifeq ($(PLATFORM),WINDOWS)
	$(TARGET)
else
	./$(TARGET)
endif

# Clean build files
clean:
ifeq ($(PLATFORM),WINDOWS)
	@if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)
	@if exist $(BIN_DIR) rmdir /S /Q $(BIN_DIR)
else
	rm -rf $(BUILD_DIR) $(BIN_DIR)
endif
	@echo Cleaned build files

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: all

# Release build
release: CXXFLAGS += -O3 -DNDEBUG
release: all

.PHONY: all clean run debug release
