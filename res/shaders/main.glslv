#version 460

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texCoord;

out vec4 a_color;
out vec2 a_texCoord;

uniform mat4 u_model;
uniform mat4 u_projview;

void main(){
	a_color = vec4(1.0f,1.0f,1.0f,1.0f);
	a_texCoord = v_texCoord;
	gl_Position = u_projview * u_model * vec4(v_position, 1.0);
}