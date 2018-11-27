/*
 * glutils.cpp
 *
 *  Created on: Nov 25, 2018
 *      Author: dsingh
 */

#include "glutils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define GL_LOG_FILE "gl.log"
#define MAX_SHADER_LENGTH 262144

bool restart_gl_log(){
	FILE *f = fopen(GL_LOG_FILE, "w");

	if(!f){
		fprintf( stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n",
								 GL_LOG_FILE );
		return false;
	}
	time_t now = time(NULL);
	char *date = ctime(&now);

	fprintf(f, "GL_LOG_FILE log. local time %s\n", date);
	fclose(f);


	return true;
}

bool gl_log(const char *message, ... ){
	va_list argptr;
	FILE *f = fopen(GL_LOG_FILE, "a");
	if(!f){
		fprintf( stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n",
										 GL_LOG_FILE );
		return false;
	}

	va_start(argptr, message);
	vfprintf(f, message, argptr);
	va_end(argptr);
	fclose(f);

	return true;
}

bool gl_log_err(const char *message, ...){

	va_list argptr;
	FILE *f = fopen(GL_LOG_FILE, "a");
	if(!f){
		fprintf( stderr, "ERROR: could not open GL_LOG_FILE log file %s for writing\n",
												 GL_LOG_FILE );
		return false;
	}

	va_start(argptr, message);
	vfprintf(f, message, argptr);
	va_end(argptr);
	fclose(f);
	//print the error on console.
	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	va_end(argptr);


	return true;
}

bool start_gl(){
	gl_log("Starting GLFW %s", glfwGetVersionString());
	glfwSetErrorCallback(glfw_error_callback);

	if(!glfwInit()){
		fprintf( stderr, "ERROR: could not start GLFW3\n" );
		return false;
	}


	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
	glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );


	g_window = glfwCreateWindow(g_gl_width, g_gl_height, "Triangle Rotation.", NULL, NULL);
	if(!g_window){
		fprintf( stderr, "ERROR: Unable to create GLFW window\n" );
		glfwTerminate();
		return false;
	}

	glfwSetFramebufferSizeCallback(g_window, glfw_framebuffer_size_callback);

	glfwMakeContextCurrent(g_window);

	glfwWindowHint( GLFW_SAMPLES, 4 );

	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte *renderer = glGetString( GL_RENDERER ); // get renderer string
	const GLubyte *version = glGetString( GL_VERSION );		// version as a string
	printf( "Renderer: %s\n", renderer );
	printf( "OpenGL version supported %s\n", version );
	gl_log( "renderer: %s\nversion: %s\n", renderer, version );

	return true;
}

void glfw_error_callback( int error, const char *description ) {
	fputs( description, stderr );
	gl_log_err( "%s\n", description );
}

// a call-back function
void glfw_framebuffer_size_callback( GLFWwindow *window, int width, int height ) {
	g_gl_width = width;
	g_gl_height = height;
	printf( "width %i height %i\n", width, height );
	/* update any perspective matrices used here */
}

void _update_fps_counter( GLFWwindow *window ){
	static double previous_seconds = glfwGetTime();
	static int frame_count;
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;
	if ( elapsed_seconds > 0.25 ) {
		previous_seconds = current_seconds;
		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];
		sprintf( tmp, "opengl @ fps: %.2f", fps );
		glfwSetWindowTitle( window, tmp );
		frame_count = 0;
	}
	frame_count++;
}

bool parse_file_into_str( const char *file_name, char *shader_str, int max_len ) {
	shader_str[0] = '\0'; // reset string
	FILE *file = fopen( file_name, "r" );
	if ( !file ) {
		gl_log_err( "ERROR: opening file for reading: %s\n", file_name );
		return false;
	}
	int current_len = 0;
	char line[2048];
	strcpy( line, "" ); // remember to clean up before using for first time!
	while ( !feof( file ) ) {
		if ( NULL != fgets( line, 2048, file ) ) {
			current_len += strlen( line ); // +1 for \n at end
			if ( current_len >= max_len ) {
				gl_log_err( "ERROR: shader length is longer than string buffer length %i\n",
										max_len );
			}
			strcat( shader_str, line );
		}
	}
	fclose( file );
	return true;
}

void print_shader_info_log( GLuint shader_index ) {
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetShaderInfoLog( shader_index, max_length, &actual_length, log );
	printf( "shader info log for GL index %i:\n%s\n", shader_index, log );
	gl_log( "shader info log for GL index %i:\n%s\n", shader_index, log );
}

void print_programme_info_log( GLuint sp ) {
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetProgramInfoLog( sp, max_length, &actual_length, log );
	printf( "program info log for GL index %u:\n%s", sp, log );
	gl_log( "program info log for GL index %u:\n%s", sp, log );
}

bool is_programme_valid( GLuint sp ) {
	glValidateProgram( sp );
	GLint params = -1;
	glGetProgramiv( sp, GL_VALIDATE_STATUS, &params );
	if ( GL_TRUE != params ) {
		gl_log_err( "program %i GL_VALIDATE_STATUS = GL_FALSE\n", sp );
		print_programme_info_log( sp );
		return false;
	}
	gl_log( "program %i GL_VALIDATE_STATUS = GL_TRUE\n", sp );
	return true;
}


bool create_shader(const char *shader_file, GLuint *shader, GLenum type){
	gl_log( "creating shader from %s...\n", shader_file );
	char shader_string[MAX_SHADER_LENGTH];
	parse_file_into_str( shader_file, shader_string, MAX_SHADER_LENGTH );
	*shader = glCreateShader(type);
	gl_log(shader_string);
	const GLchar *p = (const GLchar *) shader_string;
	//fprintf("str %s:",p);
	glShaderSource(*shader, 1, &p, NULL);
	glCompileShader(*shader);
	int params = -1;
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &params);

	if(GL_TRUE!=params){
		fprintf( stderr, "ERROR: GL vertex shader index %i did not compile\n", *shader );
		print_shader_info_log( *shader );
		return false;
	}
	gl_log( "shader compiled. index %i\n", *shader );

	return true;
}

bool create_program(GLuint vert, GLuint frag, GLuint *shader_program){
	*shader_program = glCreateProgram();
	glAttachShader(*shader_program, vert);
	glAttachShader(*shader_program, frag);
	glLinkProgram( *shader_program );
	GLint params = -1;
	glGetProgramiv( *shader_program, GL_LINK_STATUS, &params );
	if ( GL_TRUE != params ) {
		gl_log_err( "ERROR: could not link shader programme GL index %u\n",
								*shader_program );
		print_programme_info_log( *shader_program );
		return false;
	}
	( is_programme_valid( *shader_program ) );
	// delete shaders here to free memory
	glDeleteShader( vert );
	glDeleteShader( frag );
	return true;
}

GLuint create_programme_from_files( const char *vert_file_name, const char *frag_file_name ) {
	GLuint vert, frag, programme;
	( create_shader( vert_file_name, &vert, GL_VERTEX_SHADER ) );
	( create_shader( frag_file_name, &frag, GL_FRAGMENT_SHADER ) );
	( create_program( vert, frag, &programme ) );
	return programme;
}

