# Compilation instructions
CXXFLAGS = -std=c++14 -g -Wall -O1

# All object modules
OBJ = main.o Lexer.o

#Build targets
TARGETS = multi

# .PHONY: means that the targets that follow are build even though they are not real
.PHONY: all clean
all: $(TARGETS)

Lexer: $(OBJ)
	$(CXX)  -o $@ $(CXXFLAGS) $(OBJ)

#Delete .o and exe files and force recompilation.
clean:
	rm -f $(OBJ) $(TARGETS)

#Dependencies generated using directions provided in linear example

Lexer.o: Lexer.cpp Lexer.hpp
main.o: main.cpp Lexer.hpp
