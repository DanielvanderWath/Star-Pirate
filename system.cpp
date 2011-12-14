#include "system.h"

System::System(const char *Name, int numPlanets)
{
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
		for(list<Planet*>::iterator jt=it; jt!=planetList.end(); jt++)
		{
			if(jt != it && rand()%2==0)
			{
				(*it)->addLink((*jt));
			}	
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

	GLERR("colour");
	
	for(list<Planet*>::iterator it=planetList.begin(); it!=planetList.end(); it++)
	{
		GLuint uiColour=(*it)->getColour();
		GLfloat mat[]={	1.0f, 0.0f, 0.0f, (*it)->getX(),
				0.0f, 1.0f, 0.0f, (*it)->getY(),
				0.0f, 0.0f, 1.0f, (*it)->getZ(),
				0.0f, 0.0f, 0.0f, 1.0f,
				};

		GLfloat colourData[4];

		for(int i=3; i>=0; i--)
		{
			colourData[i]=((GLfloat)(uiColour&&0xFF)) / 255.0f;
				uiColour >> 8;
		}

		printf("%f %f %f %f\n", colourData[0], colourData[1], colourData[2], colourData[3]);

		glUniform4fv(colour, 1, colourData);
		glUniformMatrix4fv(matrix, 1, GL_TRUE, mat);
		GLERR("Uniform");
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	//unbind/disable the things we bound/enabled
	glDisableVertexAttribArray(position);
	glDisableVertexAttribArray(colour);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
