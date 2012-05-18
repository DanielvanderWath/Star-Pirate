#version 130
in vec3 position;
in  float texIndex;
out float ftexIndex;

uniform mat4 matrix;

void main(void)
{
	gl_Position = matrix * vec4(position, 1.0) ;
	gl_PointSize = 15.0 * (matrix[0][0] + matrix[1][1] + matrix[2][2])/5.0;

	ftexIndex=texIndex;
}
