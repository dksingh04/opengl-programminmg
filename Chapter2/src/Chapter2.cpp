//============================================================================
// Name        : Chapter2.cpp
// Author      : Deepak Singh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <string>
#include <strings.h>
#include <fstream>
#include <sstream>

using namespace std;
#define bool int
#define true 1
#define false 0
#define GL_LOG_FILE "gl.log"

 bool restart_gl_log(){
	 time_t now;
	 char *date;
	 FILE *file = fopen(GL_LOG_FILE,"w");
	 if(!file){
		 fprintf(stderr, "ERROR: Could not able to open file %s for writing \n", GL_LOG_FILE);
		 return false;
	 }
	 now = time(NULL);
	 date = ctime(&now);
	 fprintf(file, "GL_LOG_FILE log. local time %s", date);
	 fprintf(file, "build version: %s %s", __DATE__, __TIME__);
	 fclose(file);
	 return true;
 }

 bool gl_log(const char *message, ...){

	 va_list argptr;
	 FILE *file = fopen(GL_LOG_FILE, "a");

	 if(!file){
		 fprintf(stderr, "ERROR: Could not able to open file %s for writing in append mode\n", GL_LOG_FILE);
		 return false;
	 }

	 va_start(argptr, message);
	 vfprintf(file, message, argptr);
	 va_end(argptr);
	 fclose(file);

	 return true;

 }

 bool gl_log_error(const char *message, ...){
	 va_list argptr;

	 FILE *file = fopen(GL_LOG_FILE, "a");
     if(!file){
    	 fprintf(stderr, "ERROR: Could not able to open file %s for writing in append mode\n", GL_LOG_FILE);
   		 return false;
     }

     va_start(argptr, message);
     vfprintf(file, message, argptr);
     va_end(argptr);

     va_start(argptr, message);
     vfprintf(stderr, message, argptr);
     va_end(argptr);
     fclose(file);

     return true;
 }

 void glfw_error_callback(int error, const char *description){

	 gl_log_error("GLFW ERROR: code: %i msg: %s\n", error, description);

 }

int g_gl_width = 640;
int g_gl_height = 480;

void glfw_framebuffer_change_callback(GLFWwindow *window, int width, int height){
	g_gl_width = width;
	g_gl_height = height;
	printf("width: %i height: %i\n", width, height);
}

void log_gl_params(){
	GLenum params[] = {
			GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
			GL_MAX_CUBE_MAP_TEXTURE_SIZE,
			GL_MAX_DRAW_BUFFERS,
			GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
			GL_MAX_TEXTURE_IMAGE_UNITS,
			GL_MAX_TEXTURE_SIZE,
			GL_MAX_VARYING_FLOATS,
			GL_MAX_VERTEX_ATTRIBS,
			GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
			GL_MAX_VERTEX_UNIFORM_COMPONENTS,
			GL_MAX_VIEWPORT_DIMS,
			GL_STEREO,
		};
		const char *names[] = {
			"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
			"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
			"GL_MAX_DRAW_BUFFERS",
			"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
			"GL_MAX_TEXTURE_IMAGE_UNITS",
			"GL_MAX_TEXTURE_SIZE",
			"GL_MAX_VARYING_FLOATS",
			"GL_MAX_VERTEX_ATTRIBS",
			"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
			"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
			"GL_MAX_VIEWPORT_DIMS",
			"GL_STEREO",
		};

		gl_log("GL Context Params: \n");

		for(int i = 0; i < 10; i++){
			int v = 0;
			glGetIntegerv(params[i], &v);
			gl_log("%s: %i\n", names[i], v);
		}

		//others
		int v[2];
		v[0] = v[1] = 0;
		glGetIntegerv(params[10], v);

		gl_log("%s: %i %i\n", names[10], v[0], v[1]);

		unsigned char s=0;
		glGetBooleanv(params[11], &s);
		gl_log("%s: %i\n ", names[11], (unsigned int)s);
		gl_log("_________________________________\n");

}

string readShaderfile(string path){

	std::ifstream vertshaderf(path);
    std::stringstream buf;
    buf << vertshaderf.rdbuf();
    cout <<buf<<endl;

    return buf.str();
}

int main() {

	GLFWwindow *window;
	const GLubyte *renderer;
	const GLubyte *version;
	GLfloat points[] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f };
	GLuint vbo;
	GLuint vao;

	string vshaderPgm = readShaderfile("/Users/dsingh/Study/3DProgramming/3DLearning/OGLChapter1Excercise/src/test.vert");

	const char *vertex_shader = strdup(vshaderPgm.c_str());

	    //Read fragment shader from file
	string fshaderPgm = readShaderfile("/Users/dsingh/Study/3DProgramming/3DLearning/OGLChapter1Excercise/src/test.frag");

	const char *fragment_shader = strdup(fshaderPgm.c_str());

	GLuint shadder_programm, vs,fs;

	restart_gl_log();
	gl_log("Starting GLFW\n %s \n", glfwGetVersionString());

	glfwSetErrorCallback(glfw_error_callback);

	if(!glfwInit()){
		fprintf(stderr, "ERROR: Unable to start GLFW3\n");
		return 1;
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(g_gl_width, g_gl_height, "Triangle", NULL, NULL);

	if(!window){
		fprintf(stderr, "ERROR: Unable to create GLFW Window");
		glfwTerminate();
		return 1;
	}

	glfwSetFramebufferSizeCallback(window, glfw_framebuffer_change_callback);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	//get version information
	renderer = glGetString(GL_RENDERER);
	version = glGetString(GL_VERSION);

	printf("Renderer: %s\n", renderer);
	printf("Version: %s\n", version);

	gl_log("OPENGL Renderer: %s and Version: %s", renderer, version);

	log_gl_params();

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Initialize Vertex Buffer and bind.
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, 9* sizeof(GLfloat), points, GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 0, NULL);

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	shadder_programm = glCreateProgram();
	glAttachShader(shadder_programm, fs);
	glAttachShader(shadder_programm, vs);

	glLinkProgram(shadder_programm);

	while(!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glViewport(0, 0, g_gl_width, g_gl_height);

		glUseProgram(shadder_programm);

		glBindVertexArray(vao);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();

		if(GLFW_PRESS==glfwGetKey(window, GLFW_KEY_ESCAPE)){
			glfwSetWindowShouldClose(window, 1);
		}

		glfwSwapBuffers(window);

	}

	glfwTerminate();


	return 0;
}
