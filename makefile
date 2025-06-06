# Makefile for building and running Demo.cpp

# Compiler and flags
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic

# Target executable name
TARGET   := Demo

# Source files
SRC      := Demo.cpp

.PHONY: all run clean

all: $(TARGET)

# Build the demo executable
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Run the demo program
run: $(TARGET)
	./$(TARGET)

# Clean up generated files
clean:
	@rm -f $(TARGET)