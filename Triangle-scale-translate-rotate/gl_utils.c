//
//  gl_utils.c
//  03_opengl
//
//  Created by Deepak Singh on 11/7/18.
//  Copyright © 2018 Deepak Singh. All rights reserved.
//

#include "gl_utils.h"
#include <strings.h>
#include <assert.h>
#include <stdarg.h>


bool restart_gl_log(){
    time_t now;
    char *date;
    FILE *f = fopen(GL_LOG_FILE, "w");
    if(!f){
        fprintf(stderr, "ERROR: Unable to open file %s!!\n", GL_LOG_FILE);
        return false;
    }
    now = time(NULL);
    date = ctime(&now);
    
    fprintf(f, "OPEN GL Log file local time %s\n", date);
    fclose(f);
    return true;
}

bool gl_log(const char *message, ...){
    va_list argptr;
    FILE *f = fopen(GL_LOG_FILE, "a");
    if(!f){
        fprintf(stderr, "ERROR: unable to open file %s!!\n", GL_LOG_FILE);
        return false;
    }
    va_start(argptr, message);
    vfprintf(f, message, argptr);
    va_end(argptr);
    fclose(f);
    return true;
}

bool gl_error_log(const char *message, ...){
    va_list argptr;
    FILE *f = fopen(GL_LOG_FILE, "a");
    if(!f){
        fprintf(stderr, "ERROR: unable to open file %s!!\n", GL_LOG_FILE);
        return false;
    }
    va_start(argptr, message);
    vfprintf(f, message, argptr);
    va_end(argptr);
    
    va_start(argptr, message);
    vfprintf(stderr, message, argptr);
    va_end(argptr);
    
    fclose(f);
    
    return true;
}
bool start_gl(){
    
    const GLubyte *renderrer;
    const GLubyte *version;
    
    gl_log("Starting Open Gl version %s\n", glfwGetVersionString());
    
    glfwSetErrorCallback(glfw_error_callback);
    
    if(!glfwInit()){
        fprintf(stderr, "Unable to Start GLFW");
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(gl_width, gl_height, "Hello Triangle", NULL, NULL);
    if(!window){
        printf("Error: Unable to create OpenGL Window");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, glfw_framebuffer_resize_callback);
    
    glfwWindowHint( GLFW_SAMPLES, 4 );
    
    glewExperimental = GL_TRUE;
    glewInit();
    
    renderrer = glGetString(GL_RENDERER);
    version = glGetString(GL_VERSION);
    fprintf(stdout, "Renderer:%s \n Version: %s\n", renderrer, version);
    gl_log( "renderer: %s\nversion: %s\n", renderrer, version );
    
    previous_seconds = glfwGetTime();
    
    
    return true;
    
}

void glfw_error_callback(int error, const char *description){
    fputs(description, stderr);
    gl_error_log("Error Code:%v\n Error Description: %s\n", error, description);
}

void glfw_framebuffer_resize_callback(GLFWwindow *window, int width, int height){
    gl_width = width;
    gl_height = height;
    printf("Width: %i\n", gl_width);
    printf("Height: %i\n", gl_height);
}

double update_fps_counter(GLFWwindow *window){
    static int frameCount;
    double currentTime = glfwGetTime();
    double elapsedTime = currentTime - previous_seconds;
    if(elapsedTime > 0.25){
        double fps;
        char buf[128];
        previous_seconds = currentTime;
        fps = (double) frameCount/elapsedTime;
        sprintf(buf, "@fps %2f", fps);
        glfwSetWindowTitle(window, buf);
        frameCount = 0;
    }
    frameCount++;
    return elapsedTime;
}

bool load_shader_program_from_file(const char *fileName, char *shader_program, int max_len){
    FILE *f = fopen(fileName, "r");
    if(!f){
        gl_error_log("Error: Unable to open File %s\n", fileName);
        return false;
    }
    //sprintf("shaderPprogtam: %s\n", shader_program);
    size_t cnt = fread(shader_program, 1, max_len-1, f);
    fprintf(stdout, "Count %i\n",(int)cnt);
    if((int)cnt >= max_len -1){
        gl_error_log("Shader Program is to large unable to load %s\n", fileName);
        return false;
    }
    //sprintf("shaderPprogtam: %s\n", *shader_program);
    if(ferror(f)){
        gl_error_log("Error in parsing file %s \n", fileName);
        fclose(f);
        return false;
    }
    shader_program[cnt] = 0;
    fclose(f);
    return true;
}
