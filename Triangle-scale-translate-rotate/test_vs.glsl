#version 410

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_colour;

uniform mat4 matrix;
uniform mat4 scale_matrix;
uniform mat4 rot_matrix;

out vec3 colour;

void main(){
	colour = vertex_colour;
    gl_Position = matrix * rot_matrix * scale_matrix  * vec4(vertex_position, 1.0);
}
