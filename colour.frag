#version 130
uniform  vec3 data;
out vec4 colour;

void main(void)
{
	colour=vec4(data, 1.0);
}
