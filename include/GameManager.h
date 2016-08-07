#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include <GL/glew.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include "Timer.h"

#include <../WindowManager.h>

/**
 * This class handles the game logic and display.
 * Uses SDL as the display manager, and glm for 
 * vector and matrix computations
 */
class GameManager : WindowManager {
public:

	/**
	 * Constructor
	 */
	GameManager();

	/**
	 * Destructor
	 */
	~GameManager();

	/**
	 * Function that handles rendering into the OpenGL context
	 */
	void render();


	
	/**
	 * Sets states for OpenGL that we want to keep persistent
	 * throughout the game
	 */
	void setOpenGLStates();

	/**
	 * Creates the matrices for the OpenGL transformations,
	 * perspective, etc.
	 */
	void createMatrices();

	/**
	 * Compiles, attaches, links, and sets uniforms for
	 * a simple OpenGL program
	 */
	void createSimpleProgram();

	/**
	 * Creates vertex array objects
	 */
	void createVAO();

private:
	GLuint vao; //< Vertex array object
	GLuint vertex_vbo; //< VBO for vertex data
	GLuint color_vbo; //< VBO for color data
	GLuint normal_vbo; //< VBO for normal data
	GLuint program; //< OpenGL shader program

	Timer my_timer; //< Timer for machine independent motion

	glm::mat4 projection_matrix; //< OpenGL projection matrix
	glm::mat4 model_matrix; //< OpenGL model transformation matrix
	glm::mat4 view_matrix; //< OpenGL camera/view matrix
	glm::mat3 normal_matrix; //< OpenGL matrix to transfor normals
};

#endif // _GAMEMANAGER_H_
