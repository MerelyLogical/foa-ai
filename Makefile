CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++2a -O2

ifdef DEBUG
CXXFLAGS += -DDEBUG
endif

.PHONY: refresh clean

refresh: clean game

game: main.o player.o
	$(CXX) $(CXXFLAGS) main.o player.o -o game

main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o main.o

player.o: src/player.cpp
	$(CXX) $(CXXFLAGS) -c src/player.cpp -o player.o

clean:
	rm -f game *.o
