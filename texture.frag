#version 130
uniform sampler2D tex;
out vec4 colour;
in vec4 fragData;

void main(void)
{
	colour=texture2D(tex, gl_PointCoord);
	//colour = vec4(gl_PointCoord, 1.0, 1.0);
}
