TARGET = executable

COMPILER = g++

FLAGS = -Wall -g -std=c++17 -I./include -I./domain/game

LIBS = -L./lib/x64 -lfreeglut -lopengl32 -lglu32

SOURCES = main.cpp \
					domain/game/map/bsp.cpp \
					domain/game/map/map.cpp \
					domain/game/core.cpp \
					domain/game/timer.cpp \
					adapters/opengl_drawer.cpp \
					adapters/game_state_controller.cpp\
					adapters/opengl_controller.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)
	
%.o: %.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(COMPILER) $(FLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET) $(OBJECTS)
