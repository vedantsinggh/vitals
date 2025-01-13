CXX := g++
FLAGS := -Wall -Wextra -std=c++17
INCLUDE := -Iinclude
LD := -llib -Lraylib
SRC := $(wildcard src/*.cpp)

all: $(TARGET)

$(TARGET): 
	$(CXX) $(FLAGS) $(INCLUDE) -o app $(LD) $(SRC)
