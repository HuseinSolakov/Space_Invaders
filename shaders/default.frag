#version 330 core
out vec4 color;

uniform vec4 shapeColor;

void main()
{
	color = vec4(shapeColor);
}
