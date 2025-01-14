CXX := g++
FLAGS := -Wall -Wextra -std=c++17
INCLUDE := -Iinclude
LD := -Llib -lraylib

SRC := $(wildcard src/*.cpp) src/cJSON.c
OBJ := $(SRC:src/%.cpp=build/%.o) 
TARGET := build/app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(FLAGS) $(INCLUDE) -o $(TARGET) $(OBJ) $(LD)

build/%.o: src/%.cpp | build
	$(CXX) $(FLAGS) $(INCLUDE) -c $< -o $@

build/cJSON.o: src/cJSON.c | build
	gcc $(INCLUDE) -c src/cJSON.c -o build/cJSON.o

build:
	mkdir -p build

clean:
	rm -rf build
