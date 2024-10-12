#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GLES3/gl3.h>

#include <glm/glm.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "app/backend/Objects/GL/Lines.h"
#include "app/backend/Objects/GL/Shader.h"

#include "app/backend/Objects/Scene/Scene.h"


int main(int argc, char** args) {
	SDL_Window* window = NULL;

	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		return 1;
	} 

	window = SDL_CreateWindow("ComputerGraphics", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	if ( !window ) {
		return 1;
	}
	
	SDL_GLContext glContext = SDL_GL_CreateContext(window);
	
	glViewport(0, 0, 1280, 720);

	bool runningWindow = true;
	
	float counter = 0;


	SceneNamespace::Scene scene = SceneNamespace::Scene();
	
	Line* line1 = new Line(-1.0, 0.5, 0.5, -0.5);
	scene.addLine(line1);


	Line* line = new Line(1.0, 0.5, -0.5, -0.5);
	scene.addLine(line);

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

		transform = glm::rotate(transform, 0.01f, glm::vec3(0.0f, 1.0f, 1.0f));

		line->setTransformation(transform);

		scene.drawLines();

		SDL_GL_SwapWindow(window);
		counter += 0.01f;
	}

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow( window );
	SDL_Quit();
	
	return 0;
}