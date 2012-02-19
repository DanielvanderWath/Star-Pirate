#version 130
in vec3 position;
uniform  vec3 data;

out vec4 fragData;

uniform mat4 matrix;

void main(void)
{
	gl_Position = matrix * vec4(position, 1.0);
	fragData=vec4(data, 1.0);
}
