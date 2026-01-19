# Day/Night Tower Defense - Cross-platform Makefile
# Works on both Windows (MinGW) and Linux

# Detect OS
ifeq ($(OS),Windows_NT)
    PLATFORM := WINDOWS
    RM := del /Q
    MKDIR := mkdir
    EXE := .exe
    RAYLIB_PATH := raylib
else
    PLATFORM := LINUX
    RM := rm -f
    MKDIR := mkdir -p
    EXE :=
    RAYLIB_PATH := /usr/local
endif

# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17
INCLUDES := -Iinclude -I$(RAYLIB_PATH)/include
LDFLAGS := -L$(RAYLIB_PATH)/lib

# Libraries
ifeq ($(PLATFORM),WINDOWS)
    LIBS := -lraylib -lopengl32 -lgdi32 -lwinmm
else
    LIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# Directories
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

# Target executable
TARGET := $(BIN_DIR)/tower_defense$(EXE)

# Source files
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@if not exist $(BUILD_DIR) $(MKDIR) $(BUILD_DIR) 2>nul || $(MKDIR) $(BUILD_DIR)
	@if not exist $(BIN_DIR) $(MKDIR) $(BIN_DIR) 2>nul || $(MKDIR) $(BIN_DIR)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS) $(LIBS)
	@echo Build complete: $(TARGET)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Run the game
run: all
	./$(TARGET)

# Clean build files
clean:
	@if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR) 2>nul || rm -rf $(BUILD_DIR)
	@if exist $(BIN_DIR) rmdir /S /Q $(BIN_DIR) 2>nul || rm -rf $(BIN_DIR)
	@echo Cleaned build files

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: all

# Release build
release: CXXFLAGS += -O3 -DNDEBUG
release: all

.PHONY: all clean run debug release directories
