#include "GameManager.h"
#include "GeometryManager.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <assert.h>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GlTools.h"
#include "GameException.h"

GameManager::GameManager() {
	my_timer.restart();
}

GameManager::~GameManager() {
}

void GameManager::setOpenGLStates() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glClearColor(1.0, 0.0, 0.5, 1.0);
}


void GameManager::createMatrices() {

	/**
	* FIXME 4: Create proper matrices for perspective, model,
	* and view here using GLM.
	*/

	projection_matrix = glm::perspective(45.0f, window_width / (float)window_height, 0.8f, 10.f);
	model_matrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(3.0f)), glm::vec3(-0.5f));
	view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
}

void GameManager::createSimpleProgram() {

	/**
	* FIXME 3: Finish implementations of vertex and fragment shaders.
	*/

	std::string vs_src = "#version 130\n"
		"uniform mat4 projection_matrix;\n"
		"uniform mat4 model_matrix;\n"
		"uniform mat4 view_matrix;\n"
		"in  vec3 in_Position;\n"
		"in  vec3 in_Color;\n"
		"out vec3 ex_Color;\n"
		"void main() {\n"
		"	vec4 pos = view_matrix * model_matrix * vec4(in_Position, 1.0);\n"
		"	gl_Position = projection_matrix * pos;\n"
		"	ex_Color = in_Color;\n"
		"}\n";

	std::string fs_src = "#version 130\n"
		"in vec3 ex_Color;\n"
		"out vec4 res_Color;\n"
		"void main() {\n"
		"	res_Color = vec4(ex_Color, 1.0f);\n"
		"}";

	//Compile shaders, attach to program object, and link
	GLuint vs = GlTools::compileShader(vs_src, GL_VERTEX_SHADER);
	GLuint fs = GlTools::compileShader(fs_src, GL_FRAGMENT_SHADER);
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	GlTools::linkProgram(program);

	//Set uniforms for the program.

	/**
	* FIXME 5: Set the uniforms (matrices) used in the shader here
	*/

	glUseProgram(program);
	GLint projectionLocation = glGetUniformLocation(program, "projection_matrix");
	assert(projectionLocation >= 0);
	glUniformMatrix4fv(projectionLocation, 1, 0, glm::value_ptr(projection_matrix));

	GLint modelLocation = glGetUniformLocation(program, "model_matrix");
	assert(modelLocation >= 0);
	glUniformMatrix4fv(modelLocation, 1, 0, glm::value_ptr(model_matrix));

	GLint viewLocation = glGetUniformLocation(program, "view_matrix");
	assert(viewLocation >= 0);
	glUniformMatrix4fv(viewLocation, 1, 0, glm::value_ptr(view_matrix));
	glUseProgram(0);
}

void GameManager::createVAO() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	ASSERT_GL;

	//Generate VBO for the vertex data and upload our data.
	//We have n vertices, each with three coordinates (x, y, z), and
	//each coordinate is sizeof(float) bytes.
	glGenBuffers(1, &vertex_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	glBufferData(GL_ARRAY_BUFFER,
		GeometryManager::getCubeNVertices() * 3 * sizeof(float),
		GeometryManager::getCubeVertices(), GL_STATIC_DRAW);
	GLint vertexAttribLocation = glGetAttribLocation(program, "in_Position");
	assert(vertexAttribLocation >= 0);
	glVertexAttribPointer(vertexAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vertexAttribLocation);
	ASSERT_GL;


	/**
	* FIXME 6a: Create VBO for color data and upload it
	*/

	//Generate VBO for the color data and upload our data.
	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER,
		GeometryManager::getCubeNVertices() * 3 * sizeof(float),
		GeometryManager::getCubeColors(), GL_STATIC_DRAW);
	GLint colorAttribLocation = glGetAttribLocation(program, "in_Color");
	assert(colorAttribLocation >= 0);
	glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(colorAttribLocation);
	ASSERT_GL;

	//Unbind VBOs and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	ASSERT_GL;

	//Generate VBO for normal data and upload
	/**
	* FIXME 6b: Create VBO for normal data and upload it
	*/

}

void GameManager::render() {
	//Get elapsed time
	double elapsed = my_timer.elapsedAndRestart();
	float rotate_degrees = (float)elapsed * 20.0f;

	//Clear screen, and set the correct program
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

	//Rotate cube model by multiplying the model matrix with a rotation matrix
	glm::mat4 rotate_matrix = glm::rotate(glm::mat4(1.0), rotate_degrees, glm::vec3(1.0f, 1.0f, 0.0f));
	model_matrix = rotate_matrix * model_matrix;
	GLint modelLocation = glGetUniformLocation(program, "model_matrix");
	assert(modelLocation >= 0);
	glUniformMatrix4fv(modelLocation, 1, 0, glm::value_ptr(model_matrix));

	//Render geometry
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, GeometryManager::getCubeNVertices());
	glUseProgram(0);
	ASSERT_GL;
}