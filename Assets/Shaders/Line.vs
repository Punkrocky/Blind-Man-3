#version 430

layout(location = 0) in vec4 vPosition;

uniform mat4 P;

void main()
{
  gl_Position = P * vPosition;
}
