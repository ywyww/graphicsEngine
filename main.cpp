#include <iostream>

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GLES3/gl3.h>

#include <glm/glm.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "app/backend/Objects/GL/Line.h"
#include "app/backend/Objects/GL/Shader.h"
#include "app/backend/Controller.h"
#include "app/frontend/Renderer.h"

#include "include/imgui_impl_sdl2.h"
#include "include/imgui_impl_opengl3.h"
#include "include/imgui.h"


void loop(SDL_Window* window, const float& wWidth, const float& wHeight);

// scene not in main loop. put scene into controller or renderer

int main(int argc, char** args) {

    // [CONTEXT_CREATION]
	SDL_Window* window = NULL;

    if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
        throw std::runtime_error("Failed to init");
    } 

    int windowH = 900;
    int windowW = 1800;
    window = SDL_CreateWindow("CG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowW, windowH, SDL_WINDOW_OPENGL);

    if ( !window ) {
        throw std::runtime_error("Failed to create window");
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.FontGlobalScale = 1.2;
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
	const char* glsl_version = "#version 100";
    ImGui_ImplOpenGL3_Init(glsl_version);

    // END[CONTEXT_CREATION]

    
	loop(window, windowW, windowH);


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
    
void loop(SDL_Window* window, const float& wWidth, const float& wHeight)
{
    // упал lineInput
    // need to translate coordinates from SDL2 to OPENGL: window and renderArea
    bool runningWindow = true;

    SDL_Rect glRenderArea = {20, 50, 1400, 800};
    Controller* controller = new Controller();
    Renderer renderer(controller, glRenderArea.w, glRenderArea.h);

    float x, y;             // absoluteCoordinates
    float glX, glY;         // coordinates inside a viewPort (GL-like: eg -1 < x < 1)
    float belongX = 0;      // coordinates inside a viewPort (modified)
    float belongY = 0;
    
    float lastMouseClickedX = 0;
    float lastMouseClickedY = 0;

    float x1, y1, x2, y2;       // coordinates to draw a line.
    x1 = y1 = x2 = y2 = 0;

    bool isCursorVirginClicked = false;
    bool mouseDown = false;
    bool isCursorInRenderArea = false;

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

                belongX = x - glRenderArea.x;
                belongY = wHeight - glRenderArea.y - y;

                if (belongX <= glRenderArea.w && belongY <= glRenderArea.h)
                    isCursorInRenderArea = true;
                else
                    isCursorInRenderArea = false;

                if (mouseDown)
                {
                    float xRel = event.motion.xrel;
                    float yRel = event.motion.yrel;

                    controller->translateObject(xRel, -yRel, glRenderArea.w, glRenderArea.h);
                    controller->rotateObject(xRel, -yRel, glRenderArea.w, glRenderArea.h);
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && 
                event.motion.x < glRenderArea.w + glRenderArea.x && 
                event.motion.y < glRenderArea.h + glRenderArea.y && isCursorInRenderArea)
            {
                lastMouseClickedX = belongX;
                lastMouseClickedY = belongY;
                mouseDown = true;

                controller->trySetActiveNode(lastMouseClickedX, lastMouseClickedY, glRenderArea.w, glRenderArea.h);
                x1 = belongX;
                y1 = belongY;
            }
            if (event.type == SDL_MOUSEBUTTONUP && isCursorInRenderArea)
            {
                mouseDown = false;
                x2 = event.motion.x - glRenderArea.x;
                y2 = wHeight - glRenderArea.y - event.motion.y;
                
                controller->createObject(x1, y1, x2, y2, glRenderArea.w, glRenderArea.h);
            }
        }
        
        // clear imgui buffer
        glViewport(0, 0, wWidth, wHeight);
        glDisable(GL_SCISSOR_TEST);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glViewport(glRenderArea.x, glRenderArea.y, glRenderArea.w, glRenderArea.h);
        glScissor(glRenderArea.x, glRenderArea.y, glRenderArea.w, glRenderArea.h);
        glEnable(GL_SCISSOR_TEST);


		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// draw scene
        renderer.draw();

        // draw imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        renderer.drawSceneTree();
        renderer.drawStatusBar(belongX, belongY, lastMouseClickedX, lastMouseClickedY);
        renderer.drawModes();
        renderer.drawObjectPallete();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        SDL_GL_SwapWindow(window);
	}

}