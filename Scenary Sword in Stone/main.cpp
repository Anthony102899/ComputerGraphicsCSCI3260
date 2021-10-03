/*
Type your name and student ID here
	- Name: LYU, An
	- Student ID: 1155124488
*/

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"

#include "Dependencies/glm/glm/glm.hpp"
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>


GLint programID;
GLuint vaoID[8];
glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
GLfloat rotation_x = 0.0;
GLfloat rotation_y = 0.0;
glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 viewPoint = glm::vec3(0.0f, 1.0f, 6.0f);
int fireLit = 1;

void get_OpenGL_info() {
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID) {
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID) {
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string readShaderCode(const char* fileName) {
	std::ifstream meInput(fileName);
	if (!meInput.good()) {
		std::cout << "File failed to load ... " << fileName << std::endl;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders() {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	//adapter[0] = vertexShaderCode;
	std::string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;
	
	glDeleteShader(vertexShaderID);	
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void sendDataToOpenGL() {
	// TODO:
	// create 2D objects and 3D objects and/or lines (points) here and bind to VAOs & VBOs
	const GLfloat tree[] = {
		+0.5f, +0.5f, -0.5f, 1.0f, 1.0f, 1.0f,//0
		+0.25f, +0.0f, -0.25f, 0.0f, 1.0f, 0.0f,//1
		+0.75f, +0.0f, -0.25f,0.0f, 1.0f, 0.0f,//2
		+0.25f, +0.0f, -0.75f, 0.0f, 1.0f, 0.0f,//3
		+0.75f, +0.0f, -0.75f, 0.0f, 1.0f, 0.0f,//4

		+0.5f, +0.0f, -0.5f, 1.0f, 1.0f, 1.0f,//5
		+0.25f, -0.5f, -0.25f, 0.0f, 1.0f, 0.0f,//6
		+0.75f, -0.5f, -0.25f, 0.0f, 1.0f, 0.0f,//7
		+0.25f, -0.5f, -0.75f, 0.0f, 1.0f, 0.0f,//8
		+0.75f, -0.5f, -0.75f, 0.0f, 1.0f, 0.0f,//9

		+0.4f, -0.5f, -0.4f, 0.8f, 0.4f, 0.0f,//10
		+0.6f, -0.5f, -0.4f,  0.8f, 0.4f, 0.0f,//11
		+0.4f, -0.5f, -0.6f,  0.8f, 0.4f, 0.0f,//12
		+0.6f, -0.5f, -0.6f,  0.8f, 0.4f, 0.0f,//13

		+0.4f, -1.0f, -0.4f, 0.8f, 0.4f, 0.0f,//14
		+0.6f, -1.0f, -0.4f,  0.8f, 0.4f, 0.0f,
		+0.4f, -1.0f, -0.6f,  0.8f, 0.4f, 0.0f,
		+0.6f, -1.0f, -0.6f,  0.8f, 0.4f, 0.0f,
	};
	const GLfloat tree_2[] = {
		-0.5f, +0.5f, -0.5f, 1.0f, 1.0f, 1.0f,
		-0.25f, +0.0f, -0.25f, 0.0f, 1.0f, 0.0f,
		-0.75f, +0.0f, -0.25f,0.0f, 1.0f, 0.0f,
		-0.25f, +0.0f, -0.75f, 0.0f, 1.0f, 0.0f,
		-0.75f, +0.0f, -0.75f, 0.0f, 1.0f, 0.0f,

		-0.5f, +0.0f, -0.5f, 1.0f, 1.0f, 1.0f,
		-0.25f, -0.5f, -0.25f, 0.0f, 1.0f, 0.0f,
		-0.75f, -0.5f, -0.25f, 0.0f, 1.0f, 0.0f,
		-0.25f, -0.5f, -0.75f, 0.0f, 1.0f, 0.0f,
		-0.75f, -0.5f, -0.75f, 0.0f, 1.0f, 0.0f,

		-0.4f, -0.5f, -0.4f, 0.8f, 0.4f, 0.0f,
		-0.6f, -0.5f, -0.4f,  0.8f, 0.4f, 0.0f,
		-0.4f, -0.5f, -0.6f,  0.8f, 0.4f, 0.0f,
		-0.6f, -0.5f, -0.6f,  0.8f, 0.4f, 0.0f,

		-0.4f, -1.0f, -0.4f, 0.8f, 0.4f, 0.0f,
		-0.6f, -1.0f, -0.4f,  0.8f, 0.4f, 0.0f,
		-0.4f, -1.0f, -0.6f,  0.8f, 0.4f, 0.0f,
		-0.6f, -1.0f, -0.6f,  0.8f, 0.4f, 0.0f,
	};
	GLuint tree_indices[] = {
		0,1,2,
		0,1,3,
		0,2,4,
		0,3,4,
		5,6,7,
		5,6,8,
		5,7,9,
		5,8,9,
		10,11,14,
		11,14,15,
		10,12,14,
		12,14,16,
		11,13,15,
		13,15,17,
		12,13,16,
		13,16,17

	};
	const GLfloat rock[] = {
	+0.25f, -0.5f, +0.25f, 0.8f, 0.8f, 0.8f,//0
	-0.25f, -0.5f, +0.25f, 0.8f, 0.8f, 0.8f,//1
	+0.25f, -0.5f, +0.75f, 0.8f, 0.8f, 0.8f,//2
	-0.25f, -0.5f, +0.75f, 0.8f, 0.8f, 0.8f,//3
	+0.3f, -1.0f, +0.2f, 0.5f, 0.5f, 0.5f,//4
	-0.3f, -1.0f, +0.2f, 0.5f, 0.5f, 0.5f,//5
	+0.3f, -1.0f, +0.8f, 0.5f, 0.5f, 0.5f,//6
	-0.3f, -1.0f, +0.8f, 0.5f, 0.5f, 0.5f,//7
	};
	GLuint rock_indices[]{
		0,1,2,
		1,2,3,
		0,2,4,
		2,4,6,
		0,1,5,
		0,4,5,
		2,3,6,
		3,6,7,
		1,3,5,
		3,5,7
	};

	const GLfloat ground[] = {
	+2.0f, -1.0f, +2.0f,  0.0f, 0.3f, 0.0f,
	-2.0f, -1.0f, +2.0f, 0.0f, 0.3f, 0.0f,
	+2.0f, -1.0f, -2.0f, 0.0f, 0.3f, 0.0f,
	-2.0f, -1.0f, -2.0f, 0.0f, 0.3f, 0.0f,
	};
	GLuint ground_indices[] = {
		0,1,2,
		1,2,3
	};

	const GLfloat sword[] = {
		-0.05f, -0.3f, +0.5f, 1.0f, 1.0f, 1.0f,//0
		+0.05f, -0.3f, +0.5f, 1.0f, 1.0f, 1.0f,//1
		-0.05f, -0.7f, +0.5f, 1.0f, 1.0f, 1.0f,//2
		+0.05f, -0.7f, +0.5f, 1.0f, 1.0f, 1.0f,//3
		+0.0f, -0.8f, +0.5f, 1.0f, 1.0f, 1.0f,//4
		-0.1f, -0.3f, +0.5f, 0.0f, 0.0f, 1.0f,//5
		+0.1f, -0.3f, +0.5f, 0.0f, 0.0f, 1.0f,//6
		+0.0f, -0.15f, +0.5f, 0.0f, 0.0f, 1.0f,//7
		-0.033f,-0.2f, +0.5f, 0.9f, 0.5f, 0.1f,//8
		+0.033f,-0.2f, +0.5f, 0.9f, 0.5f, 0.1f,//9
		-0.033f,+0.0f, +0.5f, 0.9f, 0.5f, 0.1f,//10
		+0.033f,+0.0f, +0.5f, 0.9f, 0.5f, 0.1f,//11
	};
	GLuint sword_indices[] = {
		0,1,2,
		1,2,3,
		2,3,4,
		5,6,7,
		8,9,10,
		9,10,11
	};
	const GLfloat torch[]{
		+1.0f, -0.5f, +0.4f, 0.7f, 0.7f, 0.7f,//0
		+0.8f, -0.5f, +0.4f, 0.7f, 0.7f, 0.7f,//1
		+1.0f, -0.5f, +0.6f, 0.7f, 0.7f, 0.7f,//2
		+0.8f, -0.5f, +0.6f, 0.7f, 0.7f, 0.7f,//3
		+0.9f, -0.65f, +0.5f, 0.4f, 0.4f, 0.4f,//4
		+0.95f, -0.5f, +0.5f, 1.0f, 1.0f, 0.0f,//5 5-9 construct the flame
		+0.85f, -0.5f, +0.5f, 1.0f, 1.0f, 0.0f,//6
		+0.9f, -0.4f, +0.5f, 1.0f, 0.0f, 0.0f,//7
		+0.9f, -0.5f, +0.55f, 1.0f, 1.0f, 0.0f,//8
		+0.9f, -0.5f, +0.45f, 1.0f, 1.0f, 0.0f,//9
	};
	const GLfloat torch_2[]{
		-1.0f, -0.5f, +0.4f, 0.7f, 0.7f, 0.7f,//0
		-0.8f, -0.5f, +0.4f, 0.7f, 0.7f, 0.7f,//1
		-1.0f, -0.5f, +0.6f, 0.7f, 0.7f, 0.7f,//2
		-0.8f, -0.5f, +0.6f, 0.7f, 0.7f, 0.7f,//3
		-0.9f, -0.65f, +0.5f, 0.4f, 0.4f, 0.4f,//4
		-0.95f, -0.5f, +0.5f, 1.0f, 1.0f, 0.0f,//5
		-0.85f, -0.5f, +0.5f, 1.0f, 1.0f, 0.0f,//6
		-0.9f, -0.4f, +0.5f, 1.0f, 0.0f, 0.0f,//7
		-0.9f, -0.5f, +0.55f, 1.0f, 1.0f, 0.0f,//8
		-0.9f, -0.5f, +0.45f, 1.0f, 1.0f, 0.0f,//9
	};
	GLuint torch_indices[] = {
		0,1,2,
		1,2,3,
		0,2,4,
		0,1,4,
		1,3,4,
		2,3,4,
		5,6,7,
		7,8,9
	};

	glGenVertexArrays(8, vaoID);
	GLuint vboID[8];

	glBindVertexArray(vaoID[0]);
	glGenBuffers(1, &vboID[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tree), tree, GL_STATIC_DRAW);

	GLuint eboID;
	glGenBuffers(1, &eboID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tree_indices), tree_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(vaoID[1]);
	glGenBuffers(1, &vboID[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rock), rock, GL_STATIC_DRAW);

	GLuint ebo_rock;
	glGenBuffers(1, &ebo_rock);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_rock);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rock_indices), rock_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(vaoID[2]);
	glGenBuffers(1, &vboID[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vboID[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	GLuint ebo_ground;
	glGenBuffers(1, &ebo_ground);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_ground);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_indices), ground_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(vaoID[3]);
	glGenBuffers(1, &vboID[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vboID[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tree_2), tree_2, GL_STATIC_DRAW);

	GLuint ebo_tree2;
	glGenBuffers(1, &ebo_tree2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_tree2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tree_indices), tree_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(vaoID[4]);
	glGenBuffers(1, &vboID[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vboID[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sword), sword, GL_STATIC_DRAW);

	GLuint ebo_sword;
	glGenBuffers(1, &ebo_sword);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_sword);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sword_indices), sword_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(vaoID[5]);
	glGenBuffers(1, &vboID[5]);
	glBindBuffer(GL_ARRAY_BUFFER, vboID[5]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(torch), torch, GL_STATIC_DRAW);

	GLuint ebo_torch;
	glGenBuffers(1, &ebo_torch);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_torch);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(torch_indices), torch_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(vaoID[6]);
	glGenBuffers(1, &vboID[6]);
	glBindBuffer(GL_ARRAY_BUFFER, vboID[6]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(torch_2), torch_2, GL_STATIC_DRAW);

	GLuint ebo_torch2;
	glGenBuffers(1, &ebo_torch2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_torch2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(torch_indices), torch_indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void paintGL(void) {
	// always run
	// TODO:
	glClearColor(0.5f, 0.5f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// render your objects and control the transformation here
	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	modelTransformMatrix = glm::scale(modelTransformMatrix, scale);
	modelTransformMatrix = glm::translate(modelTransformMatrix, translation);
	modelTransformMatrix = glm::rotate(modelTransformMatrix, glm::radians(rotation_x),glm::vec3(1.0f,0.0f,0.0f));
	modelTransformMatrix = glm::rotate(modelTransformMatrix, glm::radians(rotation_y), glm::vec3(0.0f, 1.0f, 0.0f));
	GLint modelTransformMatrixUniformLocation =
		glGetUniformLocation(programID, "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
		GL_FALSE, &modelTransformMatrix[0][0]);

	glm::mat4 defaultTransformMatrix = glm::mat4(1.0f);
	defaultTransformMatrix = glm::rotate(defaultTransformMatrix, glm::radians(rotation_x), glm::vec3(1.0f, 0.0f, 0.0f));
	defaultTransformMatrix = glm::rotate(defaultTransformMatrix, glm::radians(rotation_y), glm::vec3(0.0f, 1.0f, 0.0f));
	GLint defaultTransformMatrixUniformLocation = glGetUniformLocation(programID, "defaultTransformMatrix");
	glUniformMatrix4fv(defaultTransformMatrixUniformLocation, 1,
		GL_FALSE, &defaultTransformMatrix[0][0]);

	glm::mat4 view;
	view = glm::lookAt(viewPoint,
					   glm::vec3(0.0f, 0.0f, 0.0f),
					   glm::vec3(0.0f, 1.0f, 0.0f));
	GLint viewUniformLocation = glGetUniformLocation(programID, "view");
	glUniformMatrix4fv(viewUniformLocation, 1,
		GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 0.75f, 0.1f, 20.0f);
	GLint projectionUniformLocation = glGetUniformLocation(programID, "projection");
	glUniformMatrix4fv(projectionUniformLocation, 1,
		GL_FALSE, &projection[0][0]);
	int movable = 0;
	GLint movableUniformLocation = glGetUniformLocation(programID, "movable");
	glUniform1i(movableUniformLocation, movable);

	glBindVertexArray(vaoID[0]);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);
	glBindVertexArray(vaoID[1]);
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
    glBindVertexArray(vaoID[2]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(vaoID[3]);
	glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);
	if (fireLit) { // if the sword is pulled out from stone, darken all both of the torches.
		glBindVertexArray(vaoID[5]);
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
		glBindVertexArray(vaoID[6]);
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
	}
	else {
		glBindVertexArray(vaoID[5]);
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
		glBindVertexArray(vaoID[6]);
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	}
	movable = 1; // only the sword can be translated through keyboard control
	glUniform1i(movableUniformLocation, movable);
	glBindVertexArray(vaoID[4]);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
}	

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// TODO:
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		translation = translation + glm::vec3(0.0f, 0.2f, 0.0f);
		if (translation.y >= 0.4f) {
			translation.y = 0.4f;
			fireLit = 0;
		}
		else {
			fireLit = 1;
		}
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		translation = translation + glm::vec3(0.0f, -0.2f, 0.0f);
		if (translation.y < 0.0f) {
			translation.y = 0.0f;
		}
		fireLit = 1;

	}
	if (key == GLFW_KEY_E && action == GLFW_PRESS) {
		if (translation.y == 0.4f) {
			scale = scale + glm::vec3(0.2f, 0.2f, 0.2f);
			if (scale.x >= 1.4f) {
				scale = glm::vec3(1.4f, 1.4f, 1.4f);
			}
			if (scale.x <= 0.6f) {
				scale = glm::vec3(0.6f, 0.6f, 0.6f);
			}
		}
	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		if (translation.y == 0.4f) {
			scale = scale - glm::vec3(0.2f, 0.2f, 0.2f);
			if (scale.x >= 1.4f) {
				scale = glm::vec3(1.4f, 1.4f, 1.4f);
			}
			if (scale.x <= 0.6f) {
				scale = glm::vec3(0.6f, 0.6f, 0.6f);
			}
		}
	}
	if (key == GLFW_KEY_J && action == GLFW_PRESS) {
		viewPoint.x -= 0.5f;
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS) {
		viewPoint.x += 0.5f;
	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS) {
		viewPoint.y += 0.5f;
	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS) {
		viewPoint.y -= 0.5f;
	}
	if (key == GLFW_KEY_U && action == GLFW_PRESS) {
		viewPoint.z -= 0.5f;
	}
	if (key == GLFW_KEY_O && action == GLFW_PRESS) {
		viewPoint.z += 0.5f;
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
		rotation_x += 10.0;
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		rotation_x -= 10.0;
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		rotation_y -= 10.0;
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		rotation_y += 10.0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void initializedGL(void) {
	// run only once
	// TODO:
	sendDataToOpenGL();
	installShaders();
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char* argv[]) {
	GLFWwindow* window;

	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	/* do not allow resizing */
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 600, "Assignment 1: Sword in The Stone", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback); // TODO
	// TODO: mouse callback, etc.

	/* Initialize the glew */
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	get_OpenGL_info();
	initializedGL();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		paintGL();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
