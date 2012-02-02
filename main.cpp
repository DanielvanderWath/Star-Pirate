#include "starpirate.h"
#include "system.h"
#include "planet.h"

using namespace std;

void checkKeyboard(bool *loop)
{
	SDL_PumpEvents();
	Uint8 *key=SDL_GetKeyboardState(NULL);

	if(key[SDL_SCANCODE_ESCAPE])
		*loop=false;
}

int main(int argc, char **argv)
{
	list<System*> systemList;
	SDL_Window *window=NULL;

	//init random seed
	srand(time(NULL));

	//init SDL/OpenGL
	graphicsInit(&window);

	//create system(s)
	{
		System *temp;
		char *name=NULL;
		int nameIndices[]={	rand()%numFirstNames, rand()%numSecondNames};
		int nameLength=strlen(firstName[nameIndices[0]]) + strlen(secondName[nameIndices[1]]) + 1;//+1 for the space

		name=(char*)malloc(nameLength+1); 

		sprintf(name, "%s %s", firstName[nameIndices[0]], secondName[nameIndices[1]]);
		temp = new System(name, rand()%(MAX_SYSTEM_SIZE-1) + 1);//+1 cos we don't want 0

		systemList.push_back(temp);
	}

	bool loop=true;
	while(loop)
	{
		//keyboard input
		checkKeyboard(&loop);

		//draw system
		glClearColor(0.0f, 0.2f, 0.2f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		systemList.front()->drawPlanets();
		SDL_GL_SwapWindow(window);
	}

	graphicsClean(&window);

	return 0;
}

