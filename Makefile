TARGET = executable

COMPILER = g++

FLAGS = -Wall -g

LIBS = -lGL -lGLU -lglut

SOURCES = main.cpp \
					domain/game/map/bsp.cpp \
					domain/game/map/map.cpp \
					domain/game/core.cpp \
					adapters/opengl_drawer.cpp \
					adapters/opengl_controller.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)
	
%.o: %.cpp
	$(COMPILER) $(FLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(COMPILER) $(FLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(TARGET) $(OBJECTS)
