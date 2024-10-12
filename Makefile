FLAGS = -c -Wall
CC = g++

SOURCES = main.cpp 
SOURCES += include/imgui.cpp include/imgui_widgets.cpp include/imgui_draw.cpp include/imgui_tables.cpp include/imgui_demo.cpp
SOURCES += include/imgui_impl_opengl3.cpp include/imgui_impl_sdl2.cpp
SOURCES += backend/Scene/Scene.cpp backend/Scene/SceneGroup.cpp backend/Interfaces/ISceneObjects.cpp backend/Types/Node.cpp
SOURCES += gl/Types/Shader.cpp gl/Lines.cpp

OBJECTS = $(SOURCES:.cpp=.o)
LIBS = -lSDL2 -lGL 
INC = -I/include -I/backend

EXECUTABLE = app 

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
	$(CC) $(FLAGS) $(INC) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE)
	rm -rf ./include/*.o 
	rm -rf *.o
	rm -rf backend/Scene/*.o
	rm -rf backend/Interfaces/*.o
	rm -rf backend/Types/*.o
	rm -rf gl/Types/*.o
	rm -rf gl/*.o