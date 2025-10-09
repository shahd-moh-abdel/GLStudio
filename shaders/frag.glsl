#version 330 core

in vec2 FragPos;
out vec4 FragColor;

void main()
{
  vec3 color = vec3(1.0, 0.0, 0.0);
  FragColor = vec4(color, 1.0);
}
