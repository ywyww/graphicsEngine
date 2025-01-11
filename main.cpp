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
#include "app/Camera.h"

#include "app/backend/Objects/GL/Shader.h"
#include "app/backend/Objects/GL/Point.h"
#include "app/backend/Objects/GL/Line.h"

#include "app/backend/Model.h"
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



    Camera camera = Camera();
    camera.Position = glm::vec3(0, 0, 3.0f);
    camera.updateCameraVectors();

    float fov = 45.0f;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1400.0f / 800, 0.1f, 100.0f);
    model.setProjection(projection);

    std::string filename = "/home/german/Documents/dev/source/sourceC++/CG_SDL_GL/projects/temp";

    float deltaTime = 0;
    float lastFrame = 0;

    float lastX = 750.f;
    float lastY = 750.f;
    while (runningWindow)  
    {
        float currentFrame = SDL_GetTicks64() / 1000.0f;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (deltaTime < 0.01)
            deltaTime = 0.01;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if (event.type == SDL_QUIT)
            {
                runningWindow = false;
            }

            controller.processEvent(event, wWidth, wHeight);

/*
            glm::mat4 view = model.getView();
            glm::mat4 projection = model.getProjection();

            view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.02f));
            projection = glm::perspective(glm::radians(45.0f), (float)glRenderArea.w / (float)glRenderArea.h, 0.1f, 100.0f);

            model.setView(view);
            model.setProjection(projection);
            model.setViewAndProjectionForAll();
*/
            // glm::mat4 view = camera.GetViewMatrix();
            // model.setView(view);
            // model.setViewAndProjectionForAll();

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_s)
                {
                    if (Saver::saveIntoAFile(filename, model))
                        std::cout << "Sucess to save" << std::endl;
                    else
                        std::cout << "Raised problem" << std::endl;
                }
                else if (event.key.keysym.sym == SDLK_r)
                {
                    if (Saver::readFromAFile(filename, model))
                        std::cout << "Sucess to read" << std::endl;
                    else
                        std::cout << "Raised problem" << std::endl;
                }

                // if (event.key.keysym.sym == SDLK_UP)
                // {
                //     camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
                // }
                // else if (event.key.keysym.sym == SDLK_DOWN)
                // {
                //     camera.ProcessKeyboard(CameraMovement::DOWN, deltaTime);
                // }
                // else if (event.key.keysym.sym == SDLK_RIGHT)
                // {
                //     camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
                // }
                // else if (event.key.keysym.sym == SDLK_LEFT)
                // {
                //     camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
                // }
            }
            // if (event.type = SDL_MOUSEMOTION)
            // {
            //     float xoffset = event.motion.x - lastX;
            //     float yoffset = lastY - event.motion.y; // reversed since y-coordinates go from bottom to top

            //     lastX = event.motion.x;
            //     lastY = event.motion.y;

            //     float coeff = 6;
            //     xoffset *= coeff;
            //     yoffset *= coeff;

            //     camera.ProcessMouseMovement(xoffset, 
            //                                 yoffset);
            //     std::cout << "MOUSE!" << std::endl;
            // }

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

        if (controller.rubberDrawable)
        {
            controller.rubberThread->draw();
        }

        renderer.draw();

        controller.centerPoint.draw();
        
        // draw imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        renderer.drawSceneTree();
        renderer.drawStatusBar();
        renderer.drawModes();
        renderer.drawViewState();
        renderer.drawSettings();
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        SDL_GL_SwapWindow(window);
	}

}