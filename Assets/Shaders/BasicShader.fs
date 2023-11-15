#version 430

in vec4 fcolor;
in vec2 fUVs;

out vec4 FinalColor;

uniform sampler2D Texture;
uniform vec4 fColor;

void main()
{
  vec4 Result = fcolor;//texture(Texture, fUVs).rgba;
  //Result *= fcolor;
  FinalColor = Result;
}