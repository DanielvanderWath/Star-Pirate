#version 130
uniform sampler2D tex[4];//TODO do this properly
out vec4 colour;
in float ftexIndex;

void main(void)
{
	colour=texture2D(tex[int(ftexIndex)], gl_PointCoord);
	//colour=texture2D(tex[3], gl_PointCoord);
}
