#include "starpirate.h"

bool CheckGLError(const char *string, int line, const char *file)
{//checks GL errors. returns true if error other than GL_NO_ERROR is found
	switch(glGetError())
	{
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			printf("GL_INVALID_ENUM at %s, %s:%d\n", string, file, line);
			return true;
		case GL_INVALID_VALUE:
			printf("GL_INVALID_VALUE at %s, %s:%d\n", string, file, line);
			return true;
		case GL_INVALID_OPERATION:
			printf("GL_INVALID_OPERATION at %s, %s:%d\n", string, file, line);
			return true;
		case GL_STACK_OVERFLOW:
			printf("GL_STACK_OVERFLOW at %s, %s:%d\n", string, file, line);
			return true;
		case GL_STACK_UNDERFLOW:
			printf("GL_STACK_UNDERFLOW at %s, %s:%d\n", string, file, line);
			return true;
		case GL_OUT_OF_MEMORY:
			printf("GL_OUT_OF_MEMORY at %s, %s:%d\n", string, file, line);
			return true;

	}
return false;
}

void SDLErrorAndDie(const char *string, int line, const char *file)
{
	printf("SDL Fail: %s, %s:%d\n", string, file, line);
	SDL_Quit();
}

