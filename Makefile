CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++2a -O2

ifdef DEBUG
CXXFLAGS += -DDEBUG
endif

.PHONY: refresh clean

refresh: clean game

game: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o game

clean:
	rm -f game
