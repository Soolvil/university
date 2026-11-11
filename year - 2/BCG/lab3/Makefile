CMP=g++
STD=c++23
FLAGS=-O3 -std=$(STD)
EXEC=lab
LIBS=-lSDL3
SOURCES=main.cpp glad.c

all:

run: $(EXEC)
	./$(EXEC)

build: clean $(EXEC)

$(EXEC):
	$(CMP) $(SOURCES) $(LIBS) $(FLAGS) -o $(EXEC)

clean:
	rm -f ./lab
