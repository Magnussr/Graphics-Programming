#include <GL/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include "WindowManager.h"

#include "GameException.h"

#include "include/GameManager.h"

GameManager manager;

using std::cerr;
using std::endl;

void WindowManager::createOpenGLContext() {
	/* FIXME 1: Set which version of OpenGL to use here (3.3), and allocate the correct number
	   of bits for the different color components and depth buffer, etc. */

	/* SDL_GL_SetAttribute function is used to set an OpenGL window attribute before
	   window creation. 
	   SDL_GL_SetAttribute(SDL_GLattr attr, int value):int
	   SDL_GL_SetAttribute(An enumeration of OpenGL configuration attributes, 
						   How many bits of memory a int value)	*/

	/*Set OpenGL major and minor versions */
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	/* Set OpenGL attributes */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	// Use double buffering
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);		// Use framebuffer with 16 bit depth buffer
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);		// Use framebuffer with 8 bit for red
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);		// Use framebuffer with 8 bit for green
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);		// Use framebuffer with 8 bit for blue
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);		// Use framebuffer with 8 bit for alpha
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	
	std::cerr << "Perform the FIXME tasks." << std::endl;


	// Initalize video
	main_window = SDL_CreateWindow("Westerdals - PG6200 Example OpenGL Program", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_width, window_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!main_window) {
		THROW_EXCEPTION("SDL_CreateWindow failed");
	}

	//Create OpenGL context
	main_context = SDL_GL_CreateContext(main_window);

	/* FIXME 2: Initialize GLEW here. Make sure GLEW is initialized for a 3.3 context */

	/* glewExperimental is required in openGL 3.3 to create forward compatible contexts */

	glewExperimental = GL_TRUE;
	GLenum glewErr = glewInit();
	if (glewErr != GLEW_OK) {
		std::stringstream err;
		err << "Error initializing GLEW: " << glewGetErrorString(glewErr);
		THROW_EXCEPTION(err.str());
	}

	/* Unfortunately glewInit generates an OpenGL error, but does what it's supposed to
	   (setting function pointers for core functionality). Lets do the ugly thing of
	   swallowing the error.... */
	glGetError();
}

void WindowManager::init() {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::stringstream err;
		err << "Could not initialize SDL: " << SDL_GetError();
		THROW_EXCEPTION(err.str());
	}
	atexit(SDL_Quit);

	createOpenGLContext();
	manager.setOpenGLStates();
	manager.createMatrices();
	manager.createSimpleProgram();
	manager.createVAO();
	/**
	* See FIXME 1 and 2: This will crash unless you have created a proper
	* OpenGL 3.3 context. Create the OpenGL context, and then
	* uncomment the below lines.
	*/
	/*
	createSimpleProgram();
	createVAO();
	*/
}

void WindowManager::play() {
	bool doExit = false;

	//SDL main loop
	while (!doExit) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {// poll for pending events
			switch (event.type) {
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
				case SDLK_ESCAPE:
					doExit = true;
					break;
				case SDLK_q:
					if(event.key.keysym.mod & KMOD_CTRL) //Ctrl+q
						doExit = true;
					break;
				case SDLK_RIGHT:
					manager.view_matrix = glm::translate(manager.view_matrix, glm::vec3(-0.1, 0.0, 0.0));
					break;
				case SDLK_LEFT:
					manager.view_matrix = glm::translate(manager.view_matrix, glm::vec3(0.1, 0.0, 0.0));
					break;
				case SDLK_UP:
					manager.view_matrix = glm::translate(manager.view_matrix, glm::vec3(0.0, -0.1, 0.0));
					break;
				case SDLK_DOWN:
					manager.view_matrix = glm::translate(manager.view_matrix, glm::vec3(0.0, 0.1, 0.0));
					break;
				case SDLK_PLUS:
					manager.view_matrix = glm::translate(manager.view_matrix, glm::vec3(0.0, 0.0, 0.1));
					break;
				case SDLK_MINUS:
					manager.view_matrix = glm::translate(manager.view_matrix, glm::vec3(0.0, 0.0, -0.1));
					break;
				case SDLK_1:
					manager.render_mode = manager.RENDERMODE_WIREFRAME;
					break;
				case SDLK_2:
					manager.render_mode = manager.RENDERMODE_HIDDEN_LINE;
					break;
				case SDLK_3:
					manager.render_mode = manager.RENDERMODE_FLAT;
					break;
				case SDLK_4:
					manager.render_mode = manager.RENDERMODE_PHONG;
					break;
				}
				break;
			case SDL_QUIT: //e.g., user clicks the upper right x
				doExit = true;
				break;
			}
		}

		//Render, and swap front and back buffers
		/**
		* See FIXME 1--5: The render function will seg fault unless you have a
		* proper OpenGL context and created a proper program with the
		* correct inputs. Uncomment to make sure the render function is caled
		*/
		
		manager.render();
		
		SDL_GL_SwapWindow(main_window);
	}
	quit();
}

void WindowManager::quit() {
	std::cout << "Bye bye..." << std::endl;
}
