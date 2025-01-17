#include <iostream>
#include <fstream>

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GLES3/gl3.h>

#include <glm/glm.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/projection.hpp>

#include "app/Saver.h"
#include "app/backend/Rotator.h"

#include "app/backend/Objects/GL/Shader.h"
#include "app/backend/Objects/GL/Point.h"
#include "app/backend/Objects/GL/Line.h"

#include "app/backend/Objects/Camera.h"

#include "app/backend/Model.h"
#include "app/backend/Controller.h"
#include "app/frontend/Renderer.h"
#include "app/backend/Objects/CoordinateSystem.h"

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

    glEnable(GL_PROGRAM_POINT_SIZE);

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
    // need to translate coordinates from SDL2 to OPENGL: window and renderArea
    bool runningWindow = true;

    SDL_Rect glRenderArea = {20, 50, 1400, 800};


    Model model = Model(glRenderArea);
    Controller controller = Controller(model);
    Renderer renderer = Renderer(model, controller);

    Rotator rotator = Rotator();

    rotator.view1 = glm::translate(rotator.view1, glm::vec3(0, 0, -4));

    Camera camera = Camera();

    std::string filename = "/home/german/Documents/dev/source/sourceC++/CG_SDL_GL/projects/temp";

    float f = glm::radians(0.0f);
    float t = glm::radians(0.0f);

    float zc = 30.0f;

    bool up = true;

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

            controller.processEvent(event, wWidth, wHeight);

            if (event.type == SDL_KEYDOWN)
            {
                float deltaTime = 0.1f;
                if (event.key.keysym.sym == SDLK_UP)
                {
                    camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
                }
                else if (event.key.keysym.sym == SDLK_LEFT)
                {
                    camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
                }
                else if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
                }
                

                float deltaShift = 0.7f;
                if (event.key.keysym.sym == SDLK_i)
                {
                    camera.ProcessKeyboard(Camera_Movement::TURN_UP, deltaShift);
                }
                else if (event.key.keysym.sym == SDLK_k)
                {
                    camera.ProcessKeyboard(Camera_Movement::TURN_DOWN, deltaShift);
                }
                else if (event.key.keysym.sym == SDLK_j)
                {
                    camera.ProcessKeyboard(Camera_Movement::TURN_LEFT, deltaShift);
                }
                else if (event.key.keysym.sym == SDLK_l)
                {
                    camera.ProcessKeyboard(Camera_Movement::TURN_RIGHT, deltaShift);
                }
            }
            
        }

        const ViewState& viewState = model.getViewState();
        const glm::mat4& view = rotator.getView(viewState);
        
       // const ProjectionState& projectionState = model.getProjectionState();
       // const glm::mat4& projection = rotator.getProjection(projectionState);

        controller.coordinateSystem.setView(view);
        //controller.coordinateSystem.setProjection(projection);

        model.setView(view);
        //model.setProjection(projection);
        model.setViewAndProjectionForAll();

        
        rotator.view0 = glm::rotate(rotator.view0, 0.02f, glm::vec3(1, 0, 1));
        rotator.view1 = glm::rotate(rotator.view1, 0.02f, glm::vec3(1, 0, 1));
        rotator.view5 = camera.GetViewMatrix();

        f += glm::radians(0.5f);
        t += glm::radians(0.5f);

        glm::mat4 nigger = glm::mat4(0.0f);
        nigger[0] = glm::vec4(cos(f), sin(f) * sin(t), 0, sin(f) * cos(t) / zc);
        nigger[1] = glm::vec4(0, cos(t), 0, -sin(t) / zc);
        nigger[2] = glm::vec4(sin(f), -cos(f) * sin(t), 0, -cos(f) * cos(t) / zc);
        nigger[3] = glm::vec4(0, 0, 0, 1);

        rotator.view4 = nigger;

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

        if (controller.rubberDrawable)
        {
            controller.rubberThread->draw();
        }
        
        renderer.draw();

        controller.centerPoint.draw();
        
        controller.coordinateSystem.draw();
        
        // draw imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        renderer.drawSceneTree();
        renderer.drawStatusBar();
        renderer.drawModes();
        renderer.drawViewState();
        renderer.drawProjectionState();
        renderer.drawSettings();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        SDL_GL_SwapWindow(window);
	}

}