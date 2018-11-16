//
//  main.c
//  03_opengl
//
//  Created by Deepak Singh on 11/7/18.
//  Copyright © 2018 Deepak Singh. All rights reserved.
//

#include <stdio.h>
#include "gl_utils.h"
#include <math.h>

#include <assert.h>
int gl_width = 640;
int gl_height = 480;
GLFWwindow *window = NULL;
void _print_shader_info_log( GLuint shader_index ) {
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetShaderInfoLog( shader_index, max_length, &actual_length, log );
	printf( "shader info log for GL index %i:\n%s\n", shader_index, log );
}

/* print errors in shader linking */
void _print_programme_info_log( GLuint sp ) {
	int max_length = 2048;
	int actual_length = 0;
	char log[2048];
	glGetProgramInfoLog( sp, max_length, &actual_length, log );
	printf( "program info log for GL index %i:\n%s", sp, log );
}

/* validate shader */
bool is_valid( GLuint sp ) {
	int params = -1;

	glValidateProgram( sp );
	glGetProgramiv( sp, GL_VALIDATE_STATUS, &params );
	printf( "program %i GL_VALIDATE_STATUS = %i\n", sp, params );
	if ( GL_TRUE != params ) {
		_print_programme_info_log( sp );
		return false;
	}
	return true;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    GLuint vbo, vao;
    GLfloat points [] = { 0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f};
    GLfloat colors [] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    float speed = 0.02f;
    float last_position = 0.0f;
    // Matrix to translate the object.
    float matrix[] = {
    		1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.0f, 0.0f, 1.0f
    };
    // Matrix to Scale the object.
    float scale_matrix[] = {
    		0.1f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.1f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.1f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
    };

    //Matrix to rotate the object
    float rotation_matrix[] = {

    };

    float scale_factor = 0.01f;
    char  vertex_shader[1024 * 256];
    char  fragment_shader[1024 * 256];
    
    GLuint vs, fs, shader_program;
    
    const GLchar *p;
    
    int param = -1;
    
    GLuint colorLoc;
    
    restart_gl_log();
    start_gl();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //Enable and bind Vertex buffer object
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), points, GL_STATIC_DRAW);
    
    //Colors vbo
    GLuint color_vbo;
    glGenBuffers(1, &color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);


    // Enable and bind Vertex Attribute object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    load_shader_program_from_file("test_fs.glsl", fragment_shader, 1024 * 256);
    load_shader_program_from_file("test_vs.glsl", vertex_shader, 1024 * 256);
    
    //load shader program
    vs = glCreateShader(GL_VERTEX_SHADER);
    
    p = (const GLchar *) vertex_shader;
    //fprintf("str %s:",p);
    glShaderSource(vs, 1, &p, NULL);
    glCompileShader(vs);
    
    glGetShaderiv(vs, GL_COMPILE_STATUS, &param);
    
    if(GL_TRUE!=param){
        fprintf( stderr, "ERROR: GL vertex shader index %i did not compile\n", vs );
        _print_shader_info_log( vs );
        return false;
    }
    
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    p = (const GLchar *) fragment_shader;
    
    glShaderSource(fs, 1, &p, NULL);
    glCompileShader(fs);
    
    glGetShaderiv(fs, GL_COMPILE_STATUS, &param);
    
    if(GL_TRUE!=param){
        fprintf( stderr, "ERROR: GL fragment shader index %i did not compile\n", fs );
        _print_shader_info_log( fs );
        return false;
    }
    
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);
    
    glGetShaderiv(shader_program, GL_LINK_STATUS, &param);
    if(GL_TRUE!=param){
        fprintf( stderr, "ERROR: GL shader index %i did not linked\n", vs );
        //TODO implement printing shader info log
        return false;
    }
    
    //TODO print_all implementation and is_valid
    bool result = is_valid( shader_program );
    	assert( result );
    
    colorLoc = glGetUniformLocation(shader_program, "inputColour");
    printf("colorloc %i\n", colorLoc);
    int matrixLocation = glGetUniformLocation(shader_program, "matrix");
    int scale_matrix_loc = glGetUniformLocation(shader_program, "scale_matrix");
    //assert(colorLoc > -1);
    glUseProgram(shader_program);
    glUniform4f(colorLoc, 1.0f, 0.5f, 0.0f, 1.0f);
    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);
    glUniformMatrix4fv(scale_matrix_loc, 1, GL_FALSE, scale_matrix);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);//DISPLAY
    //glFrontFace(GL_CCW);// NO TRIANGLE
    while(!glfwWindowShouldClose(window)){
        double elapsed_time = update_fps_counter(window);

        if(fabs(last_position) > 1.0f){
            speed = -speed;
            last_position = 0.0f;
       }
        //glScalef(2.0f, 0.0f, 0.0f);
		matrix[12] = elapsed_time * speed + last_position;

		last_position = matrix[12];

		// Scale in and scale out
		scale_matrix[0] += scale_factor;
		scale_matrix[5] += scale_factor;
		scale_matrix[10] += scale_factor;

		if(scale_matrix[0] > 1.0f || scale_matrix[5] > 1.0f || scale_matrix[10] > 1.0f){
			scale_matrix[0] = 0.1f;
			scale_matrix[5] = 0.1f;
			scale_matrix[10] = 0.1f;
		}

		glUseProgram(shader_program);
		//glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, matrix);
		glUniformMatrix4fv(scale_matrix_loc, 1, GL_FALSE, scale_matrix);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, gl_width, gl_height);

        glUseProgram(shader_program);
        glBindVertexArray(vao);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3 );
        glfwPollEvents();
        if ( GLFW_PRESS == glfwGetKey( window, GLFW_KEY_ESCAPE ) ) {
            glfwSetWindowShouldClose( window, 1 );
        }
        
        glfwSwapBuffers(window);
        
    }
    glfwTerminate();
    return true;
}
