CXX = g++

SRC  = $(shell ls -d *.cpp)
HEAD = $(shell ls -d *.hpp)
OBJ  = $(patsubst %.cpp, %.o, $(SRC))

CXXFLAGS = -std=c++17 -Wall -Werror
LIBS     = $(shell pkg-config --libs sdl2 SDL2_ttf SDL2_image)

release: $(OBJ)

debug: CXXFLAGS += -g
debug: $(OBJ)

clean:
	rm -rf $(OBJ)

%.o: %.cpp $(HEAD)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

built: $(OBJ)
	$(CXX) -o built $(OBJ) $(CXXFLAGS) $(LIBS)
