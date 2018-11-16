//============================================================================
// Name        : OGLChapter1Excercise.cpp
// Author      : Deepak Singh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================


#include <iostream>
#include <string>
#include <strings.h>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#define GLFW_STATIC
#include <GLFW/glfw3.h>

using namespace std;

 string readShaderfile(string path){

	std::ifstream vertshaderf(path);
    std::stringstream buf;
    buf << vertshaderf.rdbuf();
    cout <<buf<<endl;

    return buf.str();
}

int main() {
    GLfloat points[] ={
    		-0.5f, 0.5f, 0.0f,
    		0.5f, 0.5f,0.0f,
		    0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f

    };
    GLuint vao;
    GLuint vbo;
    //create another vao.
   // GLuint vao1;
    // read vertex shader from file

    string vshaderPgm = readShaderfile("/Users/dsingh/Study/3DProgramming/3DLearning/OGLChapter1Excercise/src/test.vert");

    const char *vertex_shader = strdup(vshaderPgm.c_str());

    //Read fragment shader from file
    string fshaderPgm = readShaderfile("/Users/dsingh/Study/3DProgramming/3DLearning/OGLChapter1Excercise/src/test.frag");

    const char *fragment_shader = strdup(fshaderPgm.c_str());

    GLuint vert_shader, frag_shader;

    GLuint shader_programme;

    if(!glfwInit()){
        cout << "Error: Unable to start GLFW" <<endl;
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);

    if(!window){
        cout<<"Error: Unable to Create Window!!"<<endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental=true;

    glewInit();

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);

    cout <<"Renderer: " << renderer << endl;
    cout <<"Version: " <<version << endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 18*sizeof(GLfloat), points, GL_STATIC_DRAW);

    //vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

   /* glGenVertexArrays(2, &vao1);
    glBindVertexArray(vao1);
    glEnableVertexAttribArray(0);
    */

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,  NULL);

    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader,1,&vertex_shader,NULL);

    glCompileShader(vert_shader);

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragment_shader, NULL);
    glCompileShader(frag_shader);

    shader_programme = glCreateProgram();

    glAttachShader(shader_programme, frag_shader);
    glAttachShader(shader_programme, vert_shader);

    glLinkProgram(shader_programme);

    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.6f, 0.6f, 0.8f, 1.0f);
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6); //draw Rectangle.
        //glDrawArrays(GL_LINES, 0, 6); //draw Lines, didn't draw full rectangle |-
        //glDrawArrays(GL_LINE_STRIP, 0, 6);  //draw Strip -- will draw full rectangle
        //glDrawArrays(GL_POINTS, 0, 6); //didn't draw anything why?

        glfwPollEvents();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    delete vertex_shader;
    delete fragment_shader;

    return 0;
}


