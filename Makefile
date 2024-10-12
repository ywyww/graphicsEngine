FLAGS = -c -Wall
CC = g++

SOURCES = main.cpp 
SOURCES += include/imgui.cpp include/imgui_widgets.cpp include/imgui_draw.cpp include/imgui_tables.cpp include/imgui_demo.cpp
SOURCES += include/imgui_impl_opengl3.cpp include/imgui_impl_sdl2.cpp
SOURCES += app/backend/Interfaces/ISceneObjects.cpp app/backend/Types/Node.cpp app/backend/Objects/Scene/Scene.cpp
SOURCES += app/backend/Objects/GL/Shader.cpp app/backend/Objects/GL/Lines.cpp

OBJECTS = $(SOURCES:.cpp=.o)
LIBS = -lSDL2 -lGL 
INC = -I/include
INC += -I/app/backend/Interfaces -I/app/backend/Types -I/app/backend/Objects/GL -I/app/backend/Objects/Scene

EXECUTABLE = cg_app

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
	$(CC) $(FLAGS) $(INC) $< -o $@ 

clean:
	rm -rf $(EXECUTABLE)
	rm -rf *.o
	rm -rf ./include/*.o
	rm -rf app/backend/Interfaces/*.o
	rm -rf app/backend/Types/*.o
	rm -rf app/backend/Objects/Scene/*.o
	rm -rf app/backend/Objects/GL/*.o

	

