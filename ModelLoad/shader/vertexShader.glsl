#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUV;

out vec2 outUV;

uniform mat4 _modelMatrix;
uniform mat4 _viewMatrix;
uniform mat4 _projMatrix;

void main()
{
   gl_Position = _projMatrix * _viewMatrix * _modelMatrix * vec4(aPos, 1.0);
   outUV = aUV;
};