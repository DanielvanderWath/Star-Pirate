#version 130
in vec3 position;
uniform  vec4 data;
uniform mat4 matrix;

out vec4 fragData;

void main(void)
{
	gl_Position = matrix * vec4(position, 1.0);
	fragData=data;
}
