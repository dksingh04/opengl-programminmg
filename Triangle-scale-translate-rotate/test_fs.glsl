#version 410

uniform vec4 inputColour;
in vec3 colour;
out vec4 fragColor;

void main(){
    fragColor = vec4(colour, 1.0);
}
