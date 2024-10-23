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
#include "app/frontend/Renderer.h"

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
    
	//glViewport(0, 0, 1280, 720);

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
    
void loop(SceneNamespace::Scene& scene, SDL_Window* window) // put sizes into variables
{
    bool runningWindow = true;

    Renderer renderer(scene);
    

    float x, y;
    float glX, glY;
    SDL_Rect glRenderArea = {20, 50, 900, 640};
    

    bool flag = false;
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
            if (event.type == SDL_MOUSEMOTION)
            {
                x = event.motion.x;
                y = event.motion.y;

                glX = 2 * y / 900 - 1;  // trim to the viewport
                glY = 2 * x / 640 - 1;  // trim to the viewport
            }
        }
        
        // clear imgui buffer
        glViewport(0, 0, 1280, 720);
        glDisable(GL_SCISSOR_TEST);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        

        glViewport(20, 50, 900, 640);
        glScissor(20, 50, 900, 640);
        glEnable(GL_SCISSOR_TEST);


		glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// draw scene
        scene.drawLines();


        // draw imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        renderer.drawStatusBar(x - 20, 720 - y - 50);
        renderer.drawSceneTree();
        renderer.createLine(flag, 900, 640);

        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        SDL_GL_SwapWindow(window);
	}

}