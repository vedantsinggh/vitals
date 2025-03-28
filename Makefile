CXX := g++
FLAGS := -Wall -Wextra -std=c++17
INCLUDE := -Iinclude
LD := -Llib -lraylib

SRC := $(wildcard src/*.cpp) 
OBJ := $(SRC:src/%.cpp=build/%.o) 
TARGET := build/app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(FLAGS) $(INCLUDE) -o $(TARGET) $(OBJ) $(LD) -ldl -pthread

build/%.o: src/%.cpp | build
	$(CXX) $(FLAGS) $(INCLUDE) -c $< -o $@ 

build:
	mkdir -p build

clean:
	rm -rf build
