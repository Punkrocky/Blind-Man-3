#version 430

in vec4 fColor;
//in vec2 fTexture;

out vec4 FinalColor;

//uniform sampler2D Texture;

void main()
{
  FinalColor = fColor;
}