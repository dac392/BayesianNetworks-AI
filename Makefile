# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -g -Iheaders

# Source and header directories
SRC_DIR = src
HDR_DIR = headers

# Object files directory
OBJ_DIR = obj

# Executable name
EXEC = app

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default target
all: $(EXEC)

# Rule to create object files directory
$(OBJ_DIR):
	mkdir -p $@

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(wildcard $(HDR_DIR)/*.h) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile the program
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(EXEC)

.PHONY: all clean
