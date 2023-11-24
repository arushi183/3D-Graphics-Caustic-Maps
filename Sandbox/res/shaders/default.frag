#version 330 core
out vec4 FragColor;

in vec2 temp;

void main()
{
   FragColor = vec4(temp, 0.0f, 1.0f);
}