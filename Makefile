COMPILER = g++
BINARY = pong

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.hpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))
SAVE = settings.txt log.txt

KLIBRARY_SOURCES = $(wildcard klibrary/*.cpp)
KLIBRARY_HEADERS = $(wildcard klibrary/*.hpp)
KLIBRARY_OBJECTS = $(patsubst %.cpp, %.o, $(KLIBRARY_SOURCES))

FLAGS = -std=c++11 -Wall -Wextra `pkg-config --cflags sdl2 SDL2_ttf SDL2_image`
LIBS = `pkg-config --libs sdl2 SDL2_ttf SDL2_image`

release: $(BINARY)
	./$(BINARY)

debug: FLAGS += -g
debug: $(BINARY)
	gdb ./$(BINARY)

clean:
	rm -f $(OBJECTS) $(KLIBRARY_OBJECTS) $(BINARY) $(SAVE)

$(BINARY): $(OBJECTS) $(KLIBRARY_OBJECTS)
	$(COMPILER) -o $@ $^ $(LIBS)

%.o: %.cpp $(HEADERS) $(KLIBRARY_HEADERS) Makefile
	$(COMPILER) $(FLAGS) -c -o $@ $<

