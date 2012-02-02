#include "system.h"

System::System(const char *Name, int numPlanets)
{
	planetCount=numPlanets;
	planetLinkTotal=0;

	for(int i=0; i<3; i++)
	{
		centre[i]=0;
		rotation[i]=0;
	}

	if(!Name)
	{
		printf("Name cannot be NULL, dickhead\n");
		valid=false;
		return;
	}

	name=(char*)malloc(strlen(Name));
	strcpy(name, Name);

	printf("Creating System\n\tName: %s\n", name);

	//create the planets
	for(int i=0; i< numPlanets; i++)
	{
		float location[]={(float)rand()/(INT_MAX/2) -1.0f, (float)rand()/(INT_MAX/2) -1.0f, (float)rand()/(INT_MAX/2) -1.0f};
		Planet *p = new Planet(colour[rand() % NUM_COLOURS], location);
		planetList.push_back(p);
	}

	//link them, randomly for now, TODO: link them nicely
	for(list<Planet*>::iterator it=planetList.begin(); it!=planetList.end(); it++)
	{
		for(list<Planet*>::iterator jt=it; jt!=planetList.end(); jt++)
		{
			if(jt != it && rand()%3==0)
			{
				(*it)->addLink((*jt));
			}	
		}
		planetLinkTotal+=(*it)->getLinkCount();
	}

	//and then print the links
	printf("Links:\n");
	for(list<Planet*>::iterator it=planetList.begin(); it!=planetList.end(); it++)
		(*it)->printLinks();
}

bool System::isValid()
{
	return valid;
}

int System::drawPlanets()
{
	GLuint position, colour, matrix, texture;
	GLfloat *planetCoords=NULL;
	GLuint *planetCoordsIndices=NULL;
	int planetCoordsIndex=0;
	int planetCoordsIndicesIndex=0;

	//coords of planets
	planetCoords= new GLfloat[planetCount*7];
	planetCoordsIndices= new GLuint[planetLinkTotal*2];
	if(!planetCoords || !planetCoordsIndices)
	{
		printf("Failed to allocate memory\n");
		exit(1);
	}

	GLERR("getting planets ready to draw");

	
	//draw the planets
	for(list<Planet*>::iterator it=planetList.begin(); it!=planetList.end(); it++)
	{
		GLfloat colourData[4];
		GLuint uiColour=(*it)->getColour();
		GLfloat coords[]={(*it)->getX(),(*it)->getY(),(*it)->getZ()};

		//copy planet coords to link coords array
		planetCoords[planetCoordsIndex++]=coords[0];
		planetCoords[planetCoordsIndex++]=coords[1];
		planetCoords[planetCoordsIndex++]=coords[2];

		//copy planet colour to link coords array
		for(int i=3; i>=0; i--)
		{
		planetCoords[planetCoordsIndex++]=(float)(((uiColour & (0xFF << (3-i)*8)) >> (3-i)*8) & 0xFF) /255.0f;
				uiColour >> 8;
		}

		for(list<Planet*>::iterator jt=(*it)->getLinksBegin(); jt!=(*it)->getLinksEnd(); jt++)
		{
			planetCoordsIndices[planetCoordsIndicesIndex++]=(*it)->getid();
			planetCoordsIndices[planetCoordsIndicesIndex++]=(*jt)->getid();
		}
	}

	//the VBO for the planet coords
	glBindBuffer(GL_ARRAY_BUFFER, planetVBO);
	glBufferData(GL_ARRAY_BUFFER, planetCount*sizeof(GLfloat)*7, planetCoords, GL_STREAM_DRAW);
		GLERR("glBufferData");

	//the shader program to draw planets with
	glUseProgram(planetProgram);

	texture = glGetUniformLocation(planetProgram, "tex");
	glUniform1i(texture, 0);
		GLERR("pass in texture");

	//vertex colour
	colour = glGetAttribLocation(planetProgram, "data");
	glVertexAttribPointer(colour, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*7, (void*)(sizeof(GLfloat)*4));
		GLERR("data attrib");

	//vertex position
	position = glGetAttribLocation(planetProgram, "position");
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*7, 0);
		GLERR("position attrib");

	//draw the planet sprites
	glEnableVertexAttribArray(colour);
	glEnableVertexAttribArray(position);
	glDrawArrays(GL_POINTS, 0, planetCount);
		GLERR("Drawing planets");

	//unbind/disable the things we bound/enabled to draw the planets with
	glDisableVertexAttribArray(position);
	glDisableVertexAttribArray(colour);

	//the shader program to draw links with
	glUseProgram(lineProgram);

	//vertex position
	position = glGetAttribLocation(lineProgram, "position");
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*7, 0);
	glEnableVertexAttribArray(position);

	//link colour
	colour = glGetUniformLocation(lineProgram, "data");
		GLERR("getting links ready to draw");

	glUniform3f(colour, 0.0f, 1.0f, 0.0f);

	//draw the links
	glDrawElements(GL_LINES, planetLinkTotal*2, GL_UNSIGNED_INT, planetCoordsIndices);

	glDisableVertexAttribArray(position);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLERR("Drawing lines");
}

int System::getPlanetCount()
{
	return planetCount;
}

