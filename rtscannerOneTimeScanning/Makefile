# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

# Target executable
TARGET = rtscannerOneTimeScanning

# Source file
SRC = rtscannerOneTimeScanning1.cpp

# Default rule
all: $(TARGET)

$(TARGET) : $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# Clean rule
clean: 
	rm -f $(TARGET)

# Rebuild rule
rebuild: clean all
