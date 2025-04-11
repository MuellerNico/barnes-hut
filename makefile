# Compiler and flags
CXX = g++
CXXFLAGS = -Isrc -O3 -fopenmp

# Source and executable
SRC = src/main.cpp
EXEC = a.out

# Default target
all: $(EXEC)

# Build rule
$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXEC)

# Clean rule
clean:
	rm -f $(EXEC)
