#version 430

in vec4 fColor;
in vec2 fTextureCoords;

out vec4 FinalColor;

uniform sampler2D Texture;

void main()
{
  vec4 Result = texture(Texture, fTextureCoords).rgba;
  Result.xyz *= fColor.xyz;
  FinalColor = Result;
}