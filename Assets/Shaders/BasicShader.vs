#version 430

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexture;

uniform mat4 MVP;

out vec4 fColor;
out vec2 fTextureCoords;

void main()
{
  gl_Position = MVP * vec4(vPosition, 1.0);
  fColor = vColor;
  fTextureCoords = vTexture;
}
