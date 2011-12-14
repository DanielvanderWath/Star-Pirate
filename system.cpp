#include "system.h"

System::System(const char *Name, int numPlanets)
{
	planetCount=numPlanets;
	planetLinkTotal=0;

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
			if(jt != it && rand()%2==0)
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
	GLuint position, colour, matrix;
	GLfloat *linkCoords=NULL;
	int linkCoordsIndex=0;

	//coords of planets
	linkCoords= new GLfloat[planetLinkTotal*6];

	//the shader program to draw planets with
	glUseProgram(colourProgram);

	//the VBO with the planet model
	glBindBuffer(GL_ARRAY_BUFFER, planetVBO);

	//vertex position
	position = glGetAttribLocation(colourProgram, "position");
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(position);

	//planet colour
	colour = glGetUniformLocation(colourProgram, "data");

	//matrix
	matrix = glGetUniformLocation(colourProgram, "matrix");

	GLERR("getting planets ready to draw");
	
	//draw the planets
	for(list<Planet*>::iterator it=planetList.begin(); it!=planetList.end(); it++)
	{
		GLuint uiColour=(*it)->getColour();
		GLfloat coords[]={(*it)->getX(),(*it)->getY(),(*it)->getZ()};
		GLfloat mat[]={	1.0f, 0.0f, 0.0f, coords[0],
				0.0f, 1.0f, 0.0f, coords[1],
				0.0f, 0.0f, 1.0f, coords[2],
				0.0f, 0.0f, 0.0f, 1.0f,
				};

		for(list<Planet*>::iterator jt=(*it)->getLinksBegin(); jt!=(*it)->getLinksEnd(); jt++)
		{
			//copy planet coords to link coords array
			linkCoords[linkCoordsIndex++]=coords[0];
			linkCoords[linkCoordsIndex++]=coords[1];
			linkCoords[linkCoordsIndex++]=coords[2];

			//link coords
			linkCoords[linkCoordsIndex++]=(*jt)->getX();
			linkCoords[linkCoordsIndex++]=(*jt)->getY();
			linkCoords[linkCoordsIndex++]=(*jt)->getZ();
		}

		GLfloat colourData[4];

		for(int i=3; i>=0; i--)
		{
			colourData[i]=(float)(((uiColour & (0xFF << (3-i)*8)) >> (3-i)*8) & 0xFF) /255.0f;
				uiColour >> 8;
		}

		printf("%f %f %f %f\n", colourData[0], colourData[1], colourData[2], colourData[3]);

		glUniform4fv(colour, 1, colourData);
		glUniformMatrix4fv(matrix, 1, GL_TRUE, mat);
			GLERR("Uniform");
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	//unbind/disable the things we bound/enabled to draw the planets with
	glDisableVertexAttribArray(position);
	glDisableVertexAttribArray(colour);


	//bind the link VBO and put planet coordinates into it
	glBindBuffer(GL_ARRAY_BUFFER, linkVBO);
	glBufferData(GL_ARRAY_BUFFER, planetCount * 6, linkCoords, GL_DYNAMIC_DRAW);

	//the shader program to draw links with
	glUseProgram(lineProgram);

	//vertex position
	position = glGetAttribLocation(lineProgram, "position");
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(position);

	//link colour
	colour = glGetUniformLocation(lineProgram, "data");

		GLERR("getting links ready to draw");

	glUniform3f(colour, 0.0f, 1.0f, 0.0f);

	//draw the links
	glDrawArrays(GL_LINES, 0, planetLinkTotal*2);

	glDisableVertexAttribArray(position);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLERR("Drawing lines");
}

int System::getPlanetCount()
{
	return planetCount;
}

