#include <iostream>

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GLES3/gl3.h>

#include <glm/glm.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "app/backend/Objects/GL/Line.h"
#include "app/backend/Objects/GL/Shader.h"
#include "app/backend/Objects/Scene/Scene.h"

void loop(SceneNamespace::Scene& scene, SDL_Window* window);

int main(int argc, char** args) {

	SDL_Window* window = NULL;

    if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
        throw std::runtime_error("Failed to init");
    } 
    window = SDL_CreateWindow("CG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);

    if ( !window ) {
        throw std::runtime_error("Failed to create window");
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    
	glViewport(0, 0, 1280, 720);


	SceneNamespace::Scene scene = SceneNamespace::Scene();
	Line* line1 = new Line(-1.0, 0.5, 0.5, -0.5);
	scene.addLine(line1);

	Line* line = new Line(1.0, 0.5, -0.5, -0.5);
	scene.addLine(line);


	

	loop(scene, window);




    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow( window );
    SDL_Quit();

	return 0;
}

void loop(SceneNamespace::Scene& scene, SDL_Window* window)
{
	bool runningWindow = true;

    glm::mat4x4 transform = glm::mat4x4(1.0f);

    while (runningWindow)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                runningWindow = false;
            }
                
            if (event.type == SDL_MOUSEMOTION)
            {
                float x = event.motion.xrel;
                float y = event.motion.yrel;
            }
        }

        glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        scene.drawLines();

        SDL_GL_SwapWindow(window);
	}
}