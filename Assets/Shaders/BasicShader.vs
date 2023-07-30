#version 430

layout(location = 0) in vec3 vPosition;
layout(location = 2) in vec2 vUVs;

uniform mat4 P;
uniform mat4 M;

out vec2 fUVs;

void main()
{
  gl_Position = P *  M * vec4(vPosition, 1.0);
  fUVs = vUVs;
}
