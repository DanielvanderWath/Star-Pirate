#include "starpirate.h"
#include "system.h"
#include "planet.h"

using namespace std;

//pre calculate sin and cos of 5 degrees
GLfloat s5 = sin(RADS(rotateSpeed)), c5 = cos(RADS(rotateSpeed));
//and -5 degrees
GLfloat sm5 = sin(RADS(-rotateSpeed)), cm5 = cos(RADS(-rotateSpeed));
GLfloat scaleSpeed=0.005f, scrollSpeed=0.01f;

void moveCamera()
{
	if(globalScrollCount > 0)
	{
		for(int i=0; i<16; i++)
			globalMatrix[i]+=globalMatrixSpeed[i];
		globalScrollCount--;
		if(globalScrollCount == AUTO_SCROLL_DURATION/4)
		{
			//calculate the zoomed in endpoint target
			for(int i=0; i<16; i++)
				globalMatrixSpeed[i]=(globalMatrixTarget[i] - globalMatrix[i]) / ((GLfloat)AUTO_SCROLL_DURATION*0.25f);
		}
		if( globalScrollCount == 0)
			printf("%f %f %f\n", globalMatrix[3], globalMatrix[7], globalMatrix[11]);
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

	//calculate the halfway zoomed out target
	for(int i=0; i<16; i++)
		globalMatrixSpeed[i]=(globalMatrixTarget[i] - globalMatrix[i]) / ((GLfloat)AUTO_SCROLL_DURATION*0.75f);

	//put scale into target, for use in the second half of the move
	globalMatrixTarget[0]=SYSTEM_SCALE*0.9f;
	globalMatrixTarget[5]=SYSTEM_SCALE*0.9f;
	globalMatrixTarget[10]=SYSTEM_SCALE*0.9f;

	//translate the matrix target
	multMatrices4x4(tempIdent, globalMatrixTarget);

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

void checkInput(bool *loop, list<System*>::iterator *systemCursor, list<System*> *systems)
{
	SDL_PumpEvents();
	Uint8 *key=SDL_GetKeyboardState(NULL);
	GLfloat tempCentre[3];

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
						printf("Zooming to system %s\n", (**systemCursor)->getName());
						break;
					case SDLK_q:
						if((*systemCursor) == systems->begin())
							*systemCursor = systems->end();
						(*systemCursor)--;

						zoomToPoint((**systemCursor)->getX(),(**systemCursor)->getY(),(**systemCursor)->getZ());
						break;
					case SDLK_c:
							(**systemCursor)->unOverLap();
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

	//mouse
	{
		int mouseX, mouseY;
		if(SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON(1))
			printf("Left click at %f %f\n", ((float)(mouseX * 2)/WINWIDTH) - 1.0f, 1.0f - (float)(mouseY * 2)/WINHEIGHT);
	}
}

bool moveSystemsAway(System *A, System *B, float tolerance)
{
	float Ax = A->getX(), Ay = A->getY();
	float Bx = B->getX(), By = B->getY();
	//the distance between the two systems
	float distance = sqrt((Ax-Bx)*(Ax-Bx) + (Ay-By)*(Ay-By));

	if(distance < tolerance)
	{
		//move the one furthest from the origin away
		float distanceA = sqrt(Ax*Ax + Ay*Ay), distanceB = sqrt(Ax*Ax + Ay*Ay);

		float push[3]={ Ax < Bx ? (Bx-Ax) : (Ax-Bx), Ay < By ? (By-Ay) : (Ay-By), 0.0f};

		if(distanceB > distanceA)
			B->move(push);
		else
			A->move(push);

		push[0]=-push[0];
		push[1]=-push[1];

		//A->move(push);
		return true;
	}
	return false;
}

int main(int argc, char **argv)
{
	list<System*> systemList;
	list<System*>::iterator systemCursor;
	SDL_Window *window=NULL;
	Uint32 timeStart, timeDelay;

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
	int numSystems = (rand() % (MAX_SYSTEMS-1)) +1;
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

		temp->unOverLap();

		systemList.push_back(temp);
	}

	//unoverlap systems
	if(0)
	{
		bool finished = false;
		while(!finished)
		{
			finished = true;
			for(list<System*>::iterator it=systemList.begin(); it!=systemList.end(); it++)
			{
				for(list<System*>::iterator jt=it; jt!=systemList.end(); jt++)
					if((*it)->getid()!=(*jt)->getid())
						if(moveSystemsAway((*it), (*jt), 0.2f))
							finished = false;
			}
		}
	}
	//set systemCursor to first system in list
	systemCursor=systemList.begin();

	bool loop=true;
	while(loop)
	{
		timeStart=SDL_GetTicks();

		//keyboard input
		checkInput(&loop, &systemCursor, &systemList);

		//move camera towards target position
		moveCamera();

		//draw systems
		glClearColor(0.0f, 0.2f, 0.2f, 0.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for(list<System*>::iterator it = systemList.begin(); it != systemList.end(); it++)
			(*it)->drawPlanets();

		SDL_GL_SwapWindow(window);

		timeDelay=(1000/MAX_FRAMERATE) - (SDL_GetTicks() - timeStart);
		if(timeDelay < (1000/MAX_FRAMERATE))
			SDL_Delay(timeDelay);
	}

	graphicsClean(&window);

	return 0;
}

