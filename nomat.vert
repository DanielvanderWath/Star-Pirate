#version 130
in vec3 position;
uniform  vec3 data;

out vec4 fragData;

mat4 matrix=mat4(0.5, 0.0, 0.0, 0.0,
		 0.0, 0.5, 0.0, 0.0,
		 0.0, 0.0, 0.5, 0.0,
		 0.0, 0.0, 0.0, 1.0);

void main(void)
{
	gl_Position = vec4(position, 1.0) ;
	fragData=vec4(data, 1.0);
}
