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

#include "include/imgui_impl_sdl2.h"
#include "include/imgui_impl_opengl3.h"
#include "include/imgui.h"

void loop(SceneNamespace::Scene& scene, SDL_Window* window);

int main(int argc, char** args) {

    // [CONTEXT_CREATION]
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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	const char* glsl_version = "#version 100";
    ImGui_ImplOpenGL3_Init(glsl_version);

    // END[CONTEXT_CREATION]

    // [SCENE WORK]

	SceneNamespace::Scene scene = SceneNamespace::Scene();
	Line* line1 = new Line(-1.0, 0.5, 0.5, -0.5);
	scene.addLine(line1);

	Line* line = new Line(1.0, 0.5, -0.5, -0.5);
	scene.addLine(line);

    // END[SCENE WORK]


	loop(scene, window);


    // [CONTEXT DESTROYING]
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow( window );
    SDL_Quit();
    // END[CONTEXT DESTROYING]

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
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
            {
                runningWindow = false;
            }
        }


		 ImGui_ImplOpenGL3_NewFrame();
         ImGui_ImplSDL2_NewFrame();
         ImGui::NewFrame();

             ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

             ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

		 	ImGui::End();
		
		 ImGui::EndFrame();
		 ImGui::Render();
         

		glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        scene.drawLines();
	 	
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
	}

}