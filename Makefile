# Compiler to use.
CC := g++

# Compilation flags.
CFLAGS := -c -g -Wall -Werror -Wpedantic --std=c++11

# Linker flags.
LFLAGS := -g

# Source code directory.
SRC := ./src

# Object code directory.
OBJ := ./obj

# matman - interactive matrix manipulation utility
matman: $(OBJ)/matman.o $(OBJ)/matrix.o
	$(CC) $(LFLAGS) $(OBJ)/matman.o $(OBJ)/matrix.o -o matman

$(OBJ)/matman.o: $(SRC)/matman.cpp $(SRC)/matrix.hpp
	$(CC) $(CFLAGS) $(SRC)/matman.cpp -o $(OBJ)/matman.o

$(OBJ)/matrix.o: $(SRC)/matrix.cpp $(SRC)/matrix.hpp
	$(CC) $(CFLAGS) $(SRC)/matrix.cpp -o $(OBJ)/matrix.o

.PHONY: clean doc

# Remove object files and executable to ensure next make is entire.
clean:
	rm -rf $(OBJ)/*.o matman

# Generate HTML documentation.
doc:
	doxygen doxyfile
