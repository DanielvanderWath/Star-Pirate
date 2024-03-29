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
int loadTextures();
int loadPPMIntoTexture(GLuint *texture, const char *filename);
int createProgramWith2Shaders(GLuint *program, GLuint *vert, GLuint *frag);
bool checkShaderCompileStatus(GLuint shader);
int checkProgramLinkStatus(GLuint program);
bool CheckGLError(const char *string, int line, const char *file);
void SDLErrorAndDie(const char *string, int line, const char *file);
void multMatrices4x4(const GLfloat *A, GLfloat *B);
void swapMatrices4x4(GLfloat *A, GLfloat *B);

//macros
#define PI 3.148f
#define GLERR(X) if(CheckGLError(X, __LINE__, __FILE__))return 1;
#define SDLERR(X) SDLErrorAndDie(X, __LINE__, __FILE__)
#define RADS(x) ((x*PI)/180.0f)
#define __GetTempIdent GLfloat tempIdent[]={ 	1.0f, 0.0f, 0.0f, 0.0f,	0.0f, 1.0f, 0.0f, 0.0f,	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,}
#define __LoadIdentity(X) {X[0]=1.0f;X[1]=0.0f;X[2]=0.0f;X[3]=0.0f;X[4]=0.0f;X[5]=1.0f;X[6]=0.0f;X[7]=0.0f;X[8]=0.0f;X[9]=0.0f;X[10]=1.0f;X[11]=0.0f;X[12]=0.0f;X[13]=0.0f;X[14]=0.0f;X[15]=1.0f;}

extern const int colour[];
extern const int NUM_COLOURS;

extern const char *firstName[];
extern const int numFirstNames;

extern const char *secondName[];
extern const int numSecondNames;

extern const int MAX_FRAMERATE;
extern const int MAX_SYSTEM_SIZE;
extern const int MAX_SYSTEMS;
extern const int WINWIDTH;
extern const int WINHEIGHT;
extern const int AUTO_SCROLL_DURATION;
extern const int MAX_PLANET_MOVES_BEFORE_GIVE_UP;
#if 0
extern const int PLANET_TYPE_EARTH;
extern const int PLANET_TYPE_LAVA;
extern const int PLANET_TYPE_ICE;
extern const int PLANET_TYPE_DESERT;
#else
#define PLANET_TYPE_EARTH 0
#define PLANET_TYPE_LAVA 1
#define PLANET_TYPE_ICE 2
#define PLANET_TYPE_DESERT 3
#endif
extern const int NUM_PLANET_TYPES;
extern const float SYSTEM_SCALE;

extern SDL_GLContext contextGL;

extern const GLfloat rotateSpeed;
extern GLfloat globalMatrix[16];
extern GLfloat globalMatrixTarget[16];
extern GLfloat globalMatrixTargetHalfway[16];
extern GLfloat globalMatrixSpeed[16];
extern GLfloat globalRX, globalRY;
extern int globalScrollCount;

extern GLuint planetVBO, linkVBO;
extern GLuint vertShader, fragShader, planetProgram, lineVertShader, lineFragShader, lineProgram, *planetTexture, numTextures;
#endif

