//
//  main.cpp
//  TestOpenGL1
//
//  Created by Deepak Singh on 10/24/18.
//  Copyright © 2018 Deepak Singh. All rights reserved.
//

#include <iostream>
#include <GL/glew.h>
#define GLFW_STATIC
#include <GLFW/glfw3.h>
using namespace std;

int main() {
    GLfloat points[] ={0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0};
    GLuint vao;
    GLuint vbo;
    const char *vertex_shader = "#version 410\n"
                                "in vec3 vp;"
                                "void main (){"
                                " gl_Position = vec4(vp, 1.0);"
                                "}";

    const char *fragment_shader = "#version 410\n"
                                    "out vec4 frag_colour;"
                                    "void main(){"
                                    "  frag_colour =   vec4(0.5,0.0,0.5,1.0);"
                                    "}";
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
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), points, GL_STATIC_DRAW);

    //vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);

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
        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
