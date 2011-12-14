#ifndef __SPACE_TRADER_H__
#define __SPACE_TRADER_H__
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <list>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

//functions
int graphicsInit(SDL_Window **window);
void graphicsClean(SDL_Window **window);
int loadShaderFromFile(const char* filename, GLuint *shader, GLuint shaderType);
int createProgramWith2Shaders(GLuint *program, GLuint *vert, GLuint *frag);
bool checkShaderCompileStatus(GLuint shader);
int checkProgramLinkStatus(GLuint program);
bool CheckGLError(const char *string, int line, const char *file);
void SDLErrorAndDie(const char *string, int line, const char *file);

//macros
#define GLERR(X) if(CheckGLError(X, __LINE__, __FILE__))return 1;
#define SDLERR(X) SDLErrorAndDie(X, __LINE__, __FILE__)

extern const int colour[];
extern const int NUM_COLOURS;

extern const char *firstName[];
extern const int numFirstNames;

extern const char *secondName[];
extern const int numSecondNames;

extern const int MAX_SYSTEM_SIZE;
extern const int WINWIDTH;
extern const int WINHEIGHT;

extern SDL_GLContext contextGL;

extern GLuint planetVBO, linkVBO;
extern GLuint vertShader, fragShader, colourProgram, lineVertShader, lineProgram;
#endif

