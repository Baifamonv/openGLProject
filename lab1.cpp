//HW1 Comp 371 Fall 2016
//Lab 1
//modified from http://learnopengl.com/

#include "stdafx.h"

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window,GL_TRUE);
	}
}

string buildShader(string vertex_shader_path) {
	// Read the Vertex Shader code from the file
	
	string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_shader_path, ios::in);

	if (VertexShaderStream.is_open()) {
		string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?\n", vertex_shader_path.c_str());
		getchar();
		exit(-1);
	}

	return VertexShaderCode;

}

void isShaderCompile(GLuint vertexShader) {
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error :: Shader::vertex:complilation failed" << infoLog << std::endl;
	}
}
// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Triangle", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// register the key callback with window
	
	glfwSetKeyCallback(window,key_callback);

	
	// compling shader
	string vertex_shader_path = "vertex.shader";
	string fragment_shader_path1 = "fragment1.shader";
	string fragment_shader_path2 = "fragment2.shader";
	string VertexShaderCode = buildShader(vertex_shader_path);
	string FragmentShaderCode1 = buildShader(fragment_shader_path1);
	string FragmentShaderCode2 = buildShader(fragment_shader_path2);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	char const * FragmentSourcePointer1 = FragmentShaderCode1.c_str();
	char const * FragmentSourcePointer2 = FragmentShaderCode2.c_str();
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attach shader source with to shader object
	glShaderSource(vertexShader, 1, &VertexSourcePointer, NULL);
	glCompileShader(vertexShader);
	isShaderCompile(vertexShader);

	
	// fragment Shader
	GLuint fragmentShader1;
	fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader1,1,&FragmentSourcePointer1,NULL);
	glCompileShader(fragmentShader1);
	isShaderCompile(fragmentShader1);

	// fragment Shader
	GLuint fragmentShader2;
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2, 1, &FragmentSourcePointer2, NULL);
	glCompileShader(fragmentShader2);
	isShaderCompile(fragmentShader2);

	//combine and link shaders
	GLuint shaderProgramOrange, shaderProgramYellow;
	shaderProgramOrange = glCreateProgram();
	glAttachShader(shaderProgramOrange,vertexShader);
	glAttachShader(shaderProgramOrange,fragmentShader1);
	glLinkProgram(shaderProgramOrange);

	shaderProgramYellow = glCreateProgram();
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShader2);
	glLinkProgram(shaderProgramYellow);


	// interpret the vertex data


	GLfloat firstTriangle[] = {
		0.5f,0.5f,0.0f,
		0.5f,-0.5f,0.0f,
		-0.5f,-0.5f,0.0f
	};

	GLfloat secondTriangle[] = {
		-0.5f,0.1f,0.0f,
		-0.5f,0.3f,0.0f,
		-0.1f,0.2f,0.0f
	};
	/*	
	GLuint indices[] = {
		1,2,3,
		1,0,3
	};
	*/

	GLuint VBOs[2], VAOs[2],EBO;
	glGenBuffers(2, VBOs);
	//glGenBuffers(1,&EBO);
	glGenVertexArrays(2, VAOs);
	
	// First Triangle setup
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	/*
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
	
	*/
	glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
	glEnableVertexAttribArray(0);
	//unbind the VAO
	glBindVertexArray(0);
	
	// Second Triangle setup

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(secondTriangle),secondTriangle,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GLfloat),(GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);



	//game loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		//rendering command is here
		//setting the color state
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		//clear the screen with the current state
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgramOrange);
		//draw first trianle
		glBindVertexArray(VAOs[0]);
		// draw two triangle,3->6
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
		// draw second trianle
		glUseProgram(shaderProgramYellow);

		GLfloat timeValue = glfwGetTime();
		GLfloat greenValue = (sin(timeValue)) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation(shaderProgramYellow,"ourColor");
		glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,0.5f);
		
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES,0,3);

		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader1);
	glDeleteShader(fragmentShader2);
	glDeleteVertexArrays(2,VAOs);
	glDeleteBuffers(2,VBOs);

	glfwTerminate();
	return 0;
}