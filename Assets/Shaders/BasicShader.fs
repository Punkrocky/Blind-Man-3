#version 430

in vec2 fUVs;

out vec4 FinalColor;

uniform sampler2D Texture;
uniform vec4 fColor;

void main()
{
  vec4 Result = texture(Texture, fUVs).rgba;
  Result *= fColor;
  FinalColor = Result;
}