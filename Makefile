# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -lssl -lcrypto -lz

# Source files
SOURCES = main.cpp add.cpp cat_file.cpp hash.cpp init.cpp ls_tree.cpp write_tree.cpp checkout.cpp commit.cpp log.cpp 

# Output executable
TARGET = mygit

# Default rule
all: $(TARGET)

# Rule to build the target
$(TARGET): $(SOURCES)
	$(CXX) $(SOURCES) $(CXXFLAGS) -o $(TARGET)

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)
