#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <SDL.h>

class WindowManager {
public:

	/* Initializes the game, including the OpenGL context and data required */
	void init();

	/* The main loop of the game. Runs the SDL main loop */
	void play();

	/* Quit function */
	void quit();

protected:

	/* Creates the OpenGL context using SDL */
	void createOpenGLContext();

	/* Size of the SDL window*/
	static const unsigned int window_width = 800;
	static const unsigned int window_height = 600;


private:
	SDL_Window* main_window; //< Our window handle
	SDL_GLContext main_context; //< Our opengl context handle

	//glm::mat4 view_matrix; //< OpenGL camera/view matrix
};
#endif // WINDOWMANAGER_H_

