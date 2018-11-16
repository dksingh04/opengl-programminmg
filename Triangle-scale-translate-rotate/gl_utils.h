//
//  gl_utils.h
//  03_opengl
//
//  Created by Deepak Singh on 11/7/18.
//  Copyright © 2018 Deepak Singh. All rights reserved.
//

#ifndef gl_utils_h
#define gl_utils_h

#include <stdio.h>
#include <GL/glew.h>
//#include <OpenGL/glu.h>
#include <GLFW/glfw3.h>
#include <time.h>


#define bool int
#define true 1
#define false 0
#define GL_LOG_FILE "gl.log"


static double previous_seconds;
extern int gl_width;
extern int gl_height;
extern GLFWwindow *window;

bool start_gl();
bool restart_gl_log();
bool gl_log(const char *message, ...);
bool gl_error_log(const char *message, ...);
void glfw_error_callback(int error, const char *description);
void log_gl_params();
//void update_fps_counter(GLFWwindow *window);
void glfw_framebuffer_resize_callback(GLFWwindow *window, int width, int height);
double update_fps_counter(GLFWwindow *window);

bool load_shader_program_from_file(const char *fileName, char *shader_program, int max_len);


#endif /* gl_utils_h */
