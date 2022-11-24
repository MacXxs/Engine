#version 440

out vec4 color;

in vec2 uv0;

uniform sampler2D mytexture;

void main()
{
	//color = vec4(0.87, 0.98, 0.98, 1.0); 
	color = texture2D(mytexture, uv0);
}