#include "starpirate.h"

const int colour[]={	0xFF0000FF, //red
		0xFF0800FF, //orange
		0xFFFF00FF, //yellow
		0x08FF00FF, //greeny yellow
		0x00FF00FF, //green
		0x00FF08FF, //turquoise
		0x00FFFFFF, //cyan
		0x0008FFFF, //bluey green?
		0x0000FFFF, //blue
		0x0800FFFF, //dark purple
		0xFF00FFFF, //purple
		0xFF0008FF, //violet?
		};
const int NUM_COLOURS=12;

const char *firstName[]={	"Alpha",
				"Beta",
				"Gamma",
				"Delta",
				"Epsilon",
			};
const int numFirstNames=5;

const char *secondName[]={	"Sol",
				"Centauri",
				"Zeus",
				"Hyperion",
				"Poseidon",
				"Chronos",
			};
const int numSecondNames=6;

GLfloat globalMatrix[16]={	1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
			};
GLfloat globalMatrixTarget[16]={	1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f,
				};
GLfloat globalMatrixSpeed[16]={	1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f,
			};
GLfloat globalRX=0.0f, globalRY=0.0f;
const GLfloat rotateSpeed=2.0f;

int globalScrollCount=0;

const int MAX_FRAMERATE=60;
const int MAX_SYSTEM_SIZE=10;
const int MAX_SYSTEMS=10;
const int WINWIDTH=800;
const int WINHEIGHT=600;
const int AUTO_SCROLL_DURATION=60;
const float SYSTEM_SCALE=10.0f;

SDL_GLContext contextGL;
GLuint planetVBO, linkVBO;
GLuint vertShader, fragShader, planetProgram, lineVertShader, lineFragShader, lineProgram, planetTexture;

