CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic

# Source and binary for the demo
MAIN_SRC := Demo.cpp
MAIN_EXE := demo

# Test source and executable
TEST_SRC := test.cpp
TEST_EXE := test_runner

.PHONY: Main test valgrind clean

# 'make Main' will build the demo and then run it
Main: $(MAIN_EXE)
	@echo "Running Demo..."
	./$(MAIN_EXE)

# Build the demo executable from Demo.cpp
$(MAIN_EXE): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -I. -o $(MAIN_EXE) $(MAIN_SRC)

# 'make test' – will build (if needed) and run the test executable
test: $(TEST_EXE)
	@echo "Running unit tests..."
	./$(TEST_EXE)

# Compile the test executable from test.cpp
$(TEST_EXE): $(TEST_SRC)
	@echo "Building tests..."
	$(CXX) $(CXXFLAGS) -I. -o $(TEST_EXE) $(TEST_SRC)

# 'make valgrind' – run a memory-leak check on both Demo ו–Tests
valgrind: $(MAIN_EXE) $(TEST_EXE)
	@echo "Checking Demo for memory leaks with Valgrind..."
	valgrind --leak-check=full ./$(MAIN_EXE)
	@echo "Checking Tests for memory leaks with Valgrind..."
	valgrind --leak-check=full ./$(TEST_EXE)

# 'make clean' – remove all compiled binaries and object files
clean:
	@echo "Cleaning up..."
	@rm -f $(MAIN_EXE) $(TEST_EXE) *.o