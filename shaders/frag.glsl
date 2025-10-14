#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;

void main()
{
  FragColor = texture(uTexture, TexCoord) * vec4(1.0, 0.0, 0.0, 1.0);
}
