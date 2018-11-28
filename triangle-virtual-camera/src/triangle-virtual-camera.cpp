//============================================================================
// Name        : triangle-virtual-camera.cpp
// Author      : Deepak Singh
// Version     :
// Copyright   : Your copyright notice
// Description : use OpenGL to draw triangle and rotate it around Y-axis.
//============================================================================

#include <iostream>
#include "glutils.h"
#include "mathfuncs.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define GL_LOG_FILE "gl.log"


using namespace std;

int g_gl_width = 640;
int g_gl_height = 480;
GLFWwindow *g_window = NULL;


int main() {

	restart_gl_log();

	start_gl();

	//define geometry
	GLfloat points [] = {0.0f, 0.5f, 0.0f,
						 0.5f, -0.5f, 0.0f,
						 -0.5f, -0.5f, 0.0f
						};
	GLfloat colours[] = { 1.0f, 0.0f, 0.0f,
						  0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 1.0f
						};

	GLuint points_vbo;
	GLuint color_vbo;

	glGenBuffers(1, &points_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), points, GL_STATIC_DRAW);

	glGenBuffers(1, &color_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GLfloat), colours, GL_STATIC_DRAW);

	GLuint vao;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer( GL_ARRAY_BUFFER, color_vbo );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );

	GLuint shader_programme = create_programme_from_files("/Users/dsingh/Study/3DProgramming/opengl-programminmg/3DLearning/triangle-virtual-camera/src/test_vs.glsl",
															"/Users/dsingh/Study/3DProgramming/opengl-programminmg/3DLearning/triangle-virtual-camera/src/test_fs.glsl");

	/*--------------------------create camera matrices----------------------------*/
		/* create PROJECTION MATRIX */
		// input variable
		float near = 0.1f;		// near clipping plane
		float far = 100.0f;		// far clipping plane
		float fov = 67.0f * ONE_DEG_IN_RAD; // convert 67 degrees to radians
		float aspect = (float)g_gl_width / (float)g_gl_height; // aspect ratio
		// define matrix components
		float range = tan( fov * 0.5f ) * near;
		float Sx = ( 2.0f * near ) / ( range * aspect + range * aspect );
		float Sy = near / range;
		float Sz = -( far + near ) / ( far - near );
		float Pz = -( 2.0f * far * near ) / ( far - near );
		GLfloat proj_mat[] = { Sx,0.0f, 0.0f, 0.0f,
							   0.0f, Sy,0.0f, 0.0f,
							   0.0f, 0.0f, Sz,-1.0f,
							   0.0f, 0.0f, Pz, 0.0f
							  };

		/* create VIEW MATRIX */
		float cam_speed = 2.0f;			 //x unit per second
		float cam_yaw_speed = 10.0f; // 10 degrees per second
		float cam_pos[] = { 0.0f, 0.0f, 2.0f }; // don't start at zero, or we will be too close
		float cam_yaw = 0.0f;				// y-rotation in degrees
		// Here we would have use lookat function defined in mathfuncs.h
		mat4 T =
			translate( identity_mat4(), vec3( -cam_pos[0], -cam_pos[1], -cam_pos[2] ) );
		mat4 R = rotate_y_deg( identity_mat4(), -cam_yaw );
		mat4 view_mat = R * T;

		/* get location numbers of matrices in shader programme */
		GLint view_mat_location = glGetUniformLocation( shader_programme, "view" );
		GLint proj_mat_location = glGetUniformLocation( shader_programme, "proj" );
		//GLint colorLoc = glGetUniformLocation(shader_programme, "vertex_colour");
		/* use program (make current in state machine) and set default matrix values*/
		glUseProgram( shader_programme );
		glUniformMatrix4fv( view_mat_location, 1, GL_FALSE, view_mat.m );
		glUniformMatrix4fv( proj_mat_location, 1, GL_FALSE, proj_mat );
		//glUniform4f(colorLoc, 1.0f, 0.5f, 0.0f, 1.0f);

		/*------------------------------rendering loop--------------------------------*/
			/* some rendering defaults */
			glEnable( GL_CULL_FACE ); // cull face
			glCullFace( GL_BACK );		// cull back face
			glFrontFace( GL_CW );			// GL_CCW for counter clock-wise

			while ( !glfwWindowShouldClose( g_window ) ) {
				static double previous_seconds = glfwGetTime();
				double current_seconds = glfwGetTime();
				double elapsed_seconds = current_seconds - previous_seconds;
				previous_seconds = current_seconds;

				_update_fps_counter( g_window );
				// wipe the drawing surface clear
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
				glViewport( 0, 0, g_gl_width, g_gl_height );

				glUseProgram( shader_programme );
				glBindVertexArray( vao );
				// draw points 0-3 from the currently bound VAO with current in-use shader
				glDrawArrays( GL_TRIANGLES, 0, 3 );
				// update other events like input handling
				glfwPollEvents();

				/*-----------------------------move camera here-------------------------------*/
				// control keys
				bool cam_moved = false;
				if ( glfwGetKey( g_window, GLFW_KEY_A ) ) {
					cam_pos[0] -= cam_speed * elapsed_seconds;
					cam_moved = true;
				}
				if ( glfwGetKey( g_window, GLFW_KEY_D ) ) {
					cam_pos[0] += cam_speed * elapsed_seconds;
					cam_moved = true;
				}
				if ( glfwGetKey( g_window, GLFW_KEY_PAGE_UP ) ) {
					cam_pos[1] += cam_speed * elapsed_seconds;
					cam_moved = true;
				}
				if ( glfwGetKey( g_window, GLFW_KEY_PAGE_DOWN ) ) {
					cam_pos[1] -= cam_speed * elapsed_seconds;
					cam_moved = true;
				}
				if ( glfwGetKey( g_window, GLFW_KEY_W ) ) {
					cam_pos[2] -= cam_speed * elapsed_seconds;
					cam_moved = true;
				}
				if ( glfwGetKey( g_window, GLFW_KEY_S ) ) {
					cam_pos[2] += cam_speed * elapsed_seconds;
					cam_moved = true;
				}
				if ( glfwGetKey( g_window, GLFW_KEY_LEFT ) ) {
					cam_yaw += cam_yaw_speed * elapsed_seconds;
					cam_moved = true;
				}
				if ( glfwGetKey( g_window, GLFW_KEY_RIGHT ) ) {
					cam_yaw -= cam_yaw_speed * elapsed_seconds;
					cam_moved = true;
				}
				/* update view matrix */
				if ( cam_moved ) {
					mat4 T = translate( identity_mat4(), vec3( -cam_pos[0], -cam_pos[1], -cam_pos[2] ) ); // cam translation
					mat4 R = rotate_y_deg( identity_mat4(), -cam_yaw );					//
					mat4 view_mat = R * T;
					glUniformMatrix4fv( view_mat_location, 1, GL_FALSE, view_mat.m );
				}

				if ( GLFW_PRESS == glfwGetKey( g_window, GLFW_KEY_ESCAPE ) ) {
					glfwSetWindowShouldClose( g_window, 1 );
				}
				// put the stuff we've been drawing onto the display
				glfwSwapBuffers( g_window );
			}

			// close GL context and any other GLFW resources
			glfwTerminate();

			return 0;
}
