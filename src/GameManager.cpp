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

#include "GLUtils/Glutils.hpp"

#include "GlTools.h"
#include "GameException.h"
#include "../Model.h"


using std::cerr;
using std::endl;
using GLUtils::VBO;
using GLUtils::Program;
using GLUtils::readFile;


GameManager::GameManager() {
	my_timer.restart();
	render_mode = RENDERMODE_WIREFRAME;
}

GameManager::~GameManager() {
}

void GameManager::setOpenGLStates() {
	/* glEnable lets me use GL capabilities
	   glEnable(What GL capabiliti i want to use); */
	/* GL_DEPTH_TEST turns on the depth test that checks each fragmet on the screen against the depth buffer 
	   if the fragment should get drawn */
	glEnable(GL_DEPTH_TEST);
	/* glDepthFunc specify the value used for depth buffer compairisons. 
	   compare each incoming pixel depth value with the depth value present in the depth buffer.
	   glDepthFunc(How you want the fragment to pass the dept test) 
	   GL_LEQUAL lets the fragment pass if it Passes if the incoming depth value is less than or equal
	   to the stored depth value.*/
	glDepthFunc(GL_LEQUAL);
	/* GL_CULL_FACE enables us to use glCullFace. */
	glEnable(GL_CULL_FACE);
	/* glCullFace cull polygons based on their winding in window coordinates
	   this tells the renderer that it doesn't need to render bouth faces of a triangle so we can save over 
	   50% of performance on rendering 
	   glCullFace lets me use GL CullFace abilities
	   glCullFace(What GL capabiliti i want to use);
	   GL_BACk removes all triangels that you don't see infront of the view */ 
	glCullFace(GL_BACK);
	/* glClearColor is the color of the screen
	   glClearColor(Red,Green,Blue,Transparency) */
	glClearColor(0.0, 1.0, 0.5, 1.0);
}

void GameManager::createMatrices() {

	/* FIXME 4: Create proper matrices for perspective, model, and view here using GLM. */
	projection_matrix = glm::perspective(45.0f, window_width / (float)window_height, 0.8f, 10.f);
	model_matrix = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(3.0f)), glm::vec3(-0.5f));
	view_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
}

void GameManager::createSimpleProgram() {
	std::string vertexShader_src = GLUtils::readFile("shaders/Lab3VertexShader.vert");
	std::string fragmentShader_src = GLUtils::readFile("shaders/Lab3FragmentShader.frag");

	//std::string vertexShader_src = GLUtils::readFile("shaders/AssignmentCubeVertexShader.vert");
	//std::string fragmentShader_src = GLUtils::readFile("shaders/AssignmentCubeFragmentShader.frag");
	/**
	* FIXME 3: Finish implementations of vertex and fragment shaders.
	*/

	/*std::string vs_src = "#version 130\n"
		"uniform mat4 projection_matrix;\n"
		"uniform mat4 model_matrix;\n"
		"uniform mat4 view_matrix;\n"
		"uniform mat3 normal_matrix;\n"
		"in  vec3 in_Position;\n"
		"in  vec3 in_Normal;\n"
		"in  vec3 in_Color;\n"
		"out vec3 ex_Normal\n;"
		"out vec3 ex_View;\n"
		"out vec3 ex_Light;\n"
		"flat out vec3 ex_Color;\n"
		"out vec4 res_Color;\n"
		"vec3 light_pos = vec3(0.0f, 0.0f, 0.0f);\n"
		"void main() {\n"
		"	vec4 pos = view_matrix * model_matrix * vec4(in_Position, 1.0);\n"
		"	gl_Position = projection_matrix * pos;\n"
		"	vec3 n = (normal_matrix*in_Normal);\n"
		"	vec3 v = (-pos.rgb);\n"
		"	vec3 l = (light_pos - pos.rgb);\n"
		"	vec3 h = (v+l);\n"
		"	ex_Normal = n;\n"
		"	ex_View = v;\n"
		"	ex_Light = l;\n"
		"	float diff = max(0.0f, dot(l, n));\n"
		"	float spec = pow(max(0.0f, dot(h, n)), 128.0f);\n"
		"	//ex_Color = diff*in_Color + vec3(spec);\n"
		"	ex_Color = in_Color;\n"
		"}\n";*/
	/*std::string fs_src = "#version 130\n"
		"in vec3 ex_Normal;\n"
		"in vec3 ex_View;\n"
		"in vec3 ex_Light;\n"
		"flat in vec3 ex_Color;\n"
		"out vec4 res_Color;\n"
		"void main() {\n"
		"	vec3 n = normalize(ex_Normal);\n"
		"	vec3 v = normalize(ex_View);\n"
		"	vec3 l = normalize(ex_Light);\n"
		"	vec3 h = normalize(v+l);\n"
		"	float diff = max(0.0f, dot(l, n));\n"
		"	float spec = pow(max(0.0f, dot(h, n)), 128.0f);\n"
		"	res_Color = diff*vec4(ex_Color, 1.0f) + vec4(spec);\n"
		"}";*/

	//Compile shaders, attach to program object, and link
	program.reset(new Program(vertexShader_src, fragmentShader_src));

	/*GLuint vs = GlTools::compileShader(vs_src, GL_VERTEX_SHADER);
	GLuint fs = GlTools::compileShader(fs_src, GL_FRAGMENT_SHADER);
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	GlTools::linkProgram(program);
*/
	//Set uniforms for the program.
	program->use();
	glUniformMatrix4fv(program->getUniform("projection_matrix"), 1, 0, glm::value_ptr(projection_matrix));
	program->disuse();
	/**
	* FIXME 5: Set the uniforms (matrices) used in the shader here
	*/

	/*glUseProgram(program);
	GLint projectionLocation = glGetUniformLocation(program, "projection_matrix");
	assert(projectionLocation >= 0);
	glUniformMatrix4fv(projectionLocation, 1, 0, glm::value_ptr(projection_matrix));

	GLint modelLocation = glGetUniformLocation(program, "model_matrix");
	assert(modelLocation >= 0);
	glUniformMatrix4fv(modelLocation, 1, 0, glm::value_ptr(model_matrix));

	GLint viewLocation = glGetUniformLocation(program, "view_matrix");
	assert(viewLocation >= 0);
	glUniformMatrix4fv(viewLocation, 1, 0, glm::value_ptr(view_matrix));
	glUseProgram(0);*/
}

void GameManager::createVAO() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	CHECK_GL_ERROR();

	model.reset(new Model("models/bunny.obj", false));
	model->getVertices()->bind();
	program->setAttributePointer("in_Position", 3);
	CHECK_GL_ERROR();
	//ASSERT_GL;

	//Generate VBO for the vertex data and upload our data.
	//We have n vertices, each with three coordinates (x, y, z), and
	//each coordinate is sizeof(float) bytes.
	//glGenBuffers(1, &vertex_vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo);
	//glBufferData(GL_ARRAY_BUFFER, GeometryManager::getCubeNVertices() * 3 * sizeof(float), GeometryManager::getCubeVertices(), GL_STATIC_DRAW);
	//GLint vertexAttribLocation = glGetAttribLocation(program, "in_Position");
	//assert(vertexAttribLocation >= 0);
	//glVertexAttribPointer(vertexAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(vertexAttribLocation);
	//ASSERT_GL;

	///* FIXME 6a: Create VBO for color data and upload it
	//   Generate VBO for the color data and upload our data. */
	//glGenBuffers(1, &color_vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	//glBufferData(GL_ARRAY_BUFFER, GeometryManager::getCubeNVertices() * 3 * sizeof(float), GeometryManager::getCubeColors(), GL_STATIC_DRAW);
	//GLint colorAttribLocation = glGetAttribLocation(program, "in_Color");
	//assert(colorAttribLocation >= 0);
	//glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(colorAttribLocation);
	//ASSERT_GL;

	///* FIXME 6b: Create VBO for normal data and upload it
	//   Generate VBO for normal data and upload */
	//glGenBuffers(1, &normal_vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
	//glBufferData(GL_ARRAY_BUFFER, GeometryManager::getCubeNVertices() * 3 * sizeof(float), GeometryManager::getCubeNormals(), GL_STATIC_DRAW);
	//GLint normalAttribLocation = glGetAttribLocation(program, "in_Normal");
	//assert(normalAttribLocation >= 0);
	//glVertexAttribPointer(normalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(normalAttribLocation);
	//ASSERT_GL;
	CHECK_GL_ERROR();
	//Unbind VBOs and VAO
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	vertices->unbind(); //Unbinds both vertices and normals
	glBindVertexArray(0);
	CHECK_GL_ERROR();
	//ASSERT_GL;
}

void GameManager::renderMeshRecursive(MeshPart& mesh, const std::shared_ptr<GLUtils::Program>& program,
	const glm::mat4& view_matrix, const glm::mat4& model_matrix) {
	//Create modelview matrix
	glm::mat4 meshpart_model_matrix = model_matrix*mesh.transform;
	glm::mat4 modelview_matrix = view_matrix*meshpart_model_matrix;
	glUniformMatrix4fv(program->getUniform("modelview_matrix"), 1, 0, glm::value_ptr(modelview_matrix));

	//Create normal matrix, the transpose of the inverse
	//3x3 leading submatrix of the modelview matrix
	/**
	* FIXME 1: Uncomment once you have enabled normal loading etc.
	* and use of normals in your shader
	*/
	//glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(modelview_matrix)));
	//glUniformMatrix3fv(program->getUniform("normal_matrix"), 1, 0, glm::value_ptr(normal_matrix));

	glDrawArrays(GL_TRIANGLES, mesh.first, mesh.count);
	for (unsigned int i = 0; i<mesh.children.size(); ++i)
		renderMeshRecursive(mesh.children.at(i), program, view_matrix, meshpart_model_matrix);
}

void GameManager::render() {
	//Get elapsed time
	double elapsed = my_timer.elapsedAndRestart();
	float rotate_degrees = (float)elapsed * 90.0f;

	//Clear screen, and set the correct program
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glUseProgram(program);
	program->use();

	model_matrix = glm::rotate(model_matrix, rotate_degrees, glm::vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 modelview_matrix = view_matrix*model_matrix;


	////Rotate cube model by multiplying the model matrix with a rotation matrix
	//glm::mat4 rotate_matrix = glm::rotate(glm::mat4(1.0), rotate_degrees, glm::vec3(1.0f, 1.0f, 0.0f));
	//model_matrix = rotate_matrix * model_matrix;
	//GLint modelLocation = glGetUniformLocation(program, "model_matrix");
	//assert(modelLocation >= 0);
	//glUniformMatrix4fv(modelLocation, 1, 0, glm::value_ptr(model_matrix));

	////Create normal matrix, the transpose of the inverse
	////3x3 leading submatrix of the modelview matrix
	//glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(view_matrix*model_matrix)));
	//GLint normalLocation = glGetUniformLocation(program, "normal_matrix");
	//assert(normalLocation >= 0);
	//glUniformMatrix3fv(normalLocation, 1, 0, glm::value_ptr(normal_matrix));

	//Render geometry
	glBindVertexArray(vao);
	switch(render_mode){
	case RENDERMODE_WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderMeshRecursive(model->getMesh(), program, view_matrix, model_matrix);
		break;
	case RENDERMODE_HIDDEN_LINE:
	case RENDERMODE_FLAT:
	case RENDERMODE_PHONG:
		break;
	default:
		THROW_EXCEPTION("Rendermode not supported");
	}

	glBindVertexArray(0);
	CHECK_GL_ERROR();

	//glDrawArrays(GL_TRIANGLES, 0, GeometryManager::getCubeNVertices());
	//glUseProgram(0);
	//ASSERT_GL;
}

