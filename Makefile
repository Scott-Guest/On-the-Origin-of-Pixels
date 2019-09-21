LDLIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
CXX = g++
CXXFLAGS = -std=c++1z -Wconversion -Wall -Werror -Wextra -pedantic -Wno-unused-command-line-argument
MAINS = main.cpp
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp, objs/%.o, $(filter-out $(MAINS),$(SOURCES)))
TARGET = bin/pixels
# LDFLAGS = "-L/c/SFML-2.5.1/lib"
# CPPFLAGS = "-I/c/SFML-2.5.1/include"

all:	build $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

$(OBJECTS): objs/%.o : src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< $(LDLIBS) -o $@

build:
	@mkdir -p bin
	@mkdir -p objs

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS) bin objs