#version 430

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vUVs;

uniform mat4 P;
uniform mat4 M;
uniform float DT;

out vec4 fcolor;
out vec2 fUVs;

void main()
{
  gl_Position = P *  M * vec4(vPosition, 1.0);
  //gl_Position *= DT;
  fcolor = vColor;
  fUVs = vUVs;
}
