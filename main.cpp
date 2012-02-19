#include "starpirate.h"
#include "system.h"
#include "planet.h"

using namespace std;

//pre calculate sin and cos of 5 degrees
GLfloat s5 = sin(RADS(0.1f)), c5 = cos(RADS(0.1f));
//and -5 degrees
GLfloat sm5 = sin(RADS(-0.1f)), cm5 = cos(RADS(-0.1f));

void checkKeyboard(bool *loop)
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
}

int main(int argc, char **argv)
{
	list<System*> systemList;
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

	bool loop=true;
	while(loop)
	{
		//keyboard input
		checkKeyboard(&loop);

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

