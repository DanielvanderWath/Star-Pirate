#include "starpirate.h"
#include "system.h"
#include "planet.h"

using namespace std;

//pre calculate sin and cos of 5 degrees
GLfloat rotateSpeed=0.2f;
GLfloat s5 = sin(RADS(rotateSpeed)), c5 = cos(RADS(rotateSpeed));
//and -5 degrees
GLfloat sm5 = sin(RADS(-rotateSpeed)), cm5 = cos(RADS(-rotateSpeed));
GLfloat scaleSpeed=0.005f, scrollSpeed=0.001f;

void moveCamera()
{
	if(globalScrollCount > 0)
	{
		for(int i=0; i<16; i++)
			globalMatrix[i]+=globalMatrixSpeed[i];
		globalScrollCount--;
	}
	
}

void zoomToPoint(GLfloat x, GLfloat y, GLfloat z)
{

	//translate to system
	__GetTempIdent;
	tempIdent[3]=-x;///SYSTEM_SCALE;
	tempIdent[7]=-y;///SYSTEM_SCALE;
	tempIdent[11]=-z;///SYSTEM_SCALE;

	//reset global matrix speed and target
	memset(globalMatrixSpeed, 0.0f, sizeof(GLfloat)*16);
	__LoadIdentity(globalMatrixTarget);

	//put scale into target
	globalMatrixTarget[0]=SYSTEM_SCALE;
	globalMatrixTarget[5]=SYSTEM_SCALE;
	globalMatrixTarget[10]=SYSTEM_SCALE;

	//translate the matrix target
	multMatrices4x4(tempIdent, globalMatrixTarget);

	//calculate the matrix camera's matrix speed
	for(int i=0; i<16; i++)
		globalMatrixSpeed[i]=(globalMatrixTarget[i] - globalMatrix[i]) / (GLfloat)AUTO_SCROLL_DURATION;

	//set the scroll counter
	globalScrollCount=AUTO_SCROLL_DURATION;
}

void zoomToMatrix(GLfloat *matrix)
{
	for(int i=0; i<16; i++)
	{
		globalMatrixTarget[i]=matrix[i];
		globalMatrixSpeed[i]=(matrix[i] - globalMatrix[i])/(GLfloat)AUTO_SCROLL_DURATION;
		globalScrollCount=AUTO_SCROLL_DURATION;
	}
}

void checkKeyboard(bool *loop, list<System*>::iterator *systemCursor, list<System*> *systems)
{
	SDL_PumpEvents();
	Uint8 *key=SDL_GetKeyboardState(NULL);

	if(key[SDL_SCANCODE_ESCAPE])
		*loop=false;
	if(key[SDL_SCANCODE_A])
	{
		__GetTempIdent;
		tempIdent[0]=c5;
		tempIdent[2]=s5;
		tempIdent[8]=-s5;
		tempIdent[10]=c5;
		multMatrices4x4(tempIdent, globalMatrix);
	}
	if(key[SDL_SCANCODE_D])
	{
		__GetTempIdent;
		tempIdent[0]=cm5;
		tempIdent[2]=sm5;
		tempIdent[8]=-sm5;
		tempIdent[10]=cm5;
		multMatrices4x4(tempIdent, globalMatrix);
	}
	if(key[SDL_SCANCODE_W])
	{
		__GetTempIdent;
		tempIdent[5]=c5;
		tempIdent[6]=-s5;
		tempIdent[9]=s5;
		tempIdent[10]=c5;
		multMatrices4x4(tempIdent, globalMatrix);
	}
	if(key[SDL_SCANCODE_S])
	{
		__GetTempIdent;
		tempIdent[5]=cm5;
		tempIdent[6]=-sm5;
		tempIdent[9]=sm5;
		tempIdent[10]=cm5;
		multMatrices4x4(tempIdent, globalMatrix);
	}
	if(key[SDL_SCANCODE_DOWN])
	{
		__GetTempIdent;
		tempIdent[7]-=scrollSpeed;
		swapMatrices4x4(tempIdent, globalMatrix);
		multMatrices4x4(globalMatrix, tempIdent);
		swapMatrices4x4(tempIdent, globalMatrix);
	}
	if(key[SDL_SCANCODE_UP])
	{
		__GetTempIdent;
		tempIdent[7]+=scrollSpeed;
		swapMatrices4x4(tempIdent, globalMatrix);
		multMatrices4x4(globalMatrix, tempIdent);
		swapMatrices4x4(tempIdent, globalMatrix);
	}
	if(key[SDL_SCANCODE_LEFT])
	{
		__GetTempIdent;
		tempIdent[3]-=scrollSpeed;
		swapMatrices4x4(tempIdent, globalMatrix);
		multMatrices4x4(globalMatrix, tempIdent);
		swapMatrices4x4(tempIdent, globalMatrix);
	}
	if(key[SDL_SCANCODE_RIGHT])
	{
		__GetTempIdent;
		tempIdent[3]+=scrollSpeed;
		swapMatrices4x4(tempIdent, globalMatrix);
		multMatrices4x4(globalMatrix, tempIdent);
		swapMatrices4x4(tempIdent, globalMatrix);
	}
	if(key[SDL_SCANCODE_R])
	{
		__GetTempIdent;
		tempIdent[0]=1.0f+scaleSpeed;
		tempIdent[5]=1.0f+scaleSpeed;
		tempIdent[10]=1.0f+scaleSpeed;
		multMatrices4x4(tempIdent, globalMatrix);
	}
	if(key[SDL_SCANCODE_F])
	{
		__GetTempIdent;
		tempIdent[0]=1.0f-scaleSpeed;
		tempIdent[5]=1.0f-scaleSpeed;
		tempIdent[10]=1.0f-scaleSpeed;
		multMatrices4x4(tempIdent, globalMatrix);
	}

	SDL_Event keyEvent;
	if(SDL_PollEvent(&keyEvent))
	{
		switch(keyEvent.type)
		{
			case SDL_KEYDOWN:
				switch(keyEvent.key.keysym.sym)
				{
					case SDLK_e:
						(*systemCursor)++;
						if((*systemCursor) == systems->end())
							*systemCursor = systems->begin();

						zoomToPoint((**systemCursor)->getX(),(**systemCursor)->getY(),(**systemCursor)->getZ());
						break;
					case SDLK_q:
						(*systemCursor)--;
						if((*systemCursor) == systems->begin())
							*systemCursor = systems->end();

						zoomToPoint((**systemCursor)->getX(),(**systemCursor)->getY(),(**systemCursor)->getZ());
						break;
					case SDLK_SPACE:
						{
							__GetTempIdent;
							zoomToMatrix(tempIdent);
						}
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
}

int main(int argc, char **argv)
{
	list<System*> systemList;
	list<System*>::iterator systemCursor;
	SDL_Window *window=NULL;

	//init random seed
	srand(time(NULL));

	//init SDL/OpenGL
	graphicsInit(&window);

	//temporarily load and bind a texture
	glActiveTexture(GL_TEXTURE0);
	if(loadPPMIntoTexture(&planetTexture, "planet1.ppm"))
	{
		graphicsClean(&window);
		return 1;
	}

	//create system(s)
	int numSystems = rand() % MAX_SYSTEMS;
	for(int i=0; i< numSystems; i++)
	{
		System *temp;
		char *name=NULL;
		float centre[]={	((float)rand()/((float)RAND_MAX/2)) -1.0f ,((float)rand()/((float)RAND_MAX/2))-1.0f,0.0f};
		int nameIndices[]={	rand()%numFirstNames, rand()%numSecondNames};
		int nameLength=strlen(firstName[nameIndices[0]]) + strlen(secondName[nameIndices[1]]) + 1;//+1 for the space

		name=(char*)malloc(nameLength+1); 

		sprintf(name, "%s %s", firstName[nameIndices[0]], secondName[nameIndices[1]]);
		temp = new System(name, rand()%(MAX_SYSTEM_SIZE-1) + 1, centre);//+1 cos we don't want 0

		systemList.push_back(temp);
	}
	//set systemCursor to first system in list
	systemCursor=systemList.begin();

	bool loop=true;
	while(loop)
	{
		//keyboard input
		checkKeyboard(&loop, &systemCursor, &systemList);

		//move camera towards target position
		moveCamera();

		//draw systems
		glClearColor(0.0f, 0.2f, 0.2f, 0.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for(list<System*>::iterator it = systemList.begin(); it != systemList.end(); it++)
			(*it)->drawPlanets();

		SDL_GL_SwapWindow(window);
	}

	graphicsClean(&window);

	return 0;
}

