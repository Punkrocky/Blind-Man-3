#version 430

in vec4 fColor;
in vec2 fTextureCoords;
in float fIndex;

out vec4 FinalColor;

uniform sampler2D Texture[5];

void main()
{
  int index = int(fIndex);
  vec4 Result = texture(Texture[index], fTextureCoords).rgba;
  Result.xyz *= fColor.xyz;
  FinalColor = Result;
}