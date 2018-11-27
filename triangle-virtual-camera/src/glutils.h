/*
 * glutils.h
 *
 *  Created on: Nov 25, 2018
 *      Author: dsingh
 */

#ifndef GLUTILS_H_
#define GLUTILS_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdarg.h>
#define GL_LOG_FILE "gl.log"

// keep track of window size for things like the viewport and the mouse cursor
extern int g_gl_width;
extern int g_gl_height;
extern GLFWwindow *g_window;

bool start_gl();

bool restart_gl_log();

bool gl_log(const char *message, ...);

bool gl_log_err(const char *message, ...);

void glfw_error_callback( int error, const char *description );

// a call-back function
void glfw_framebuffer_size_callback( GLFWwindow *window, int width, int height );

void log_gl_params();

void _update_fps_counter( GLFWwindow *window );

void print_shader_info_log( GLuint shader_index );

void print_programme_info_log( GLuint sp );

const char *GL_type_to_string( unsigned int type );

void print_all( GLuint sp );

bool is_valid( GLuint sp );

bool parse_file_into_str( const char *file_name, char *shader_str, int max_len );

GLuint create_programme_from_files( const char *vert_file_name, const char *frag_file_name );

#endif /* GLUTILS_H_ */
