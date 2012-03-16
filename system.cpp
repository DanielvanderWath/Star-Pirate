#include "system.h"

bool moveAway(Planet *A, Planet *B, float tolerance)
{
	//move two planets away from each other if they are closer than tolerance
	float Ax = A->getX(), Ay = A->getY();
	float Bx = B->getX(), By = B->getY();
	float distance = sqrt((Ax-Bx)*(Ax-Bx) + (Ay-By)*(Ay-By));

	if(distance < tolerance)
	{
		float push[3]={ Ax < Bx ? (Bx-Ax)/2.0f : (Ax-Bx)/2.0f, Ay < By ? (By-Ay)/2.0f : (Ay-By)/2.0f, 0.0f};

		B->move(push);

		push[0]=-push[0];
		push[1]=-push[1];

		A->move(push);
		return true;
	}
	return false;
}

System::System(const char *Name, int numPlanets, float *syscentre)
{
	planetCount=0;
	planetLinkTotal=0;

	for(int i=0; i<3; i++)
	{
		centre[i]=syscentre[i];
		rotation[i]=0.0f;
	}

	if(!Name)
	{
		printf("Name cannot be NULL\n");
		valid=false;
		return;
	}

	name=(char*)malloc(strlen(Name));
	strcpy(name, Name);

	printf("Creating System\n\tName: %s\n", name);

	//create the planets
	for(int i=0; i< numPlanets; i++)
	{
		float location[]={(float)rand()/(INT_MAX/2) -1.0f, (float)rand()/(INT_MAX/2) -1.0f, 0.5f};
		Planet *p = new Planet(colour[rand() % NUM_COLOURS], location, &planetCount);
		planetList.push_back(p);
	}


	//find the centre of the arrangement of planets, and move them all so that their centre matches the system's
	float left=0.0f, right=0.0f, top=0.0f, bottom=0.0f;
	for(list<Planet*>::iterator it=planetList.begin(); it!=planetList.end(); it++)
	{
		float x, y;
		x=-(*it)->getX();
		y=-(*it)->getY();

		if(x<left)
			left=x;
		else if(x>right)
			right=x;

		if(y<bottom)
			bottom=y;
		else if(y>top)
			top=y;
	}
	float actualcentre[3];
	actualcentre[0]=(right+left)/2;
	actualcentre[1]=(top+bottom)/2;
	actualcentre[2]=0.0f;
	for(list<Planet*>::iterator it=planetList.begin(); it!=planetList.end(); it++)
	{
		(*it)->move(actualcentre);
	}

	//link them, randomly for now, TODO: link them nicely
	for(list<Planet*>::iterator it=planetList.begin(); it!=planetList.end(); it++)
	{
		for(list<Planet*>::iterator jt=it; jt!=planetList.end(); jt++)
		{
			if(jt != it && rand()%3==0)
			{
				//(*it)->addLink((*jt));
				links.push_back(new Link((*it), (*jt)));
				planetLinkTotal++;
			}	
		}
	}

	//and then print the links
/*	printf("Links:\n");
	for(list<Link*>::iterator it=links.begin(); it!=links.end(); it++)
		(*it)->print();
*/
	//and then the system centre here
	printf("System centre: %f %f %f\n", centre[0], centre[1], centre[2]);
}

void System::unOverLap()
{
	bool finished = false;
	while(!finished)
	//make sure none of them overlap
	for(list<Planet*>::iterator it=planetList.begin(); it!=planetList.end(); it++)
	{
		//set to false if we find an overlapping pair
		finished = true;
		for(list<Planet*>::iterator jt=it; jt!=planetList.end(); jt++)
			if(it!=jt)
				if(moveAway((*it), (*jt), 0.2f))
					finished=false;
	}

}

bool System::isValid()
{
	return valid;
}

int System::drawPlanets()
{
	GLuint position, phase, colour, matrix, texture;
	GLfloat *planetCoords=NULL;
	GLuint *planetCoordsIndices=NULL;
	int planetCoordsIndex=0;
	int planetCoordsIndicesIndex=0;
	//rather than calculate the transformation to the system's location
	//twice, just do it once. That's why this is declared here, rather
	//than once before each glUniformMatrix4fv
	GLfloat tempGlobal[16];

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
		planetCoords[planetCoordsIndex++]=coords[0]/SYSTEM_SCALE;
		planetCoords[planetCoordsIndex++]=coords[1]/SYSTEM_SCALE;
		planetCoords[planetCoordsIndex++]=coords[2]/SYSTEM_SCALE;

		//copy planet colour to link coords array
		for(int i=3; i>=0; i--)
		{
		planetCoords[planetCoordsIndex++]=(float)(((uiColour & (0xFF << (3-i)*8)) >> (3-i)*8) & 0xFF) /255.0f;
				uiColour >> 8;
		}

	}

	//get the links ready
	for(list<Link*>::iterator it=links.begin(); it!=links.end(); it++)
	{
		planetCoordsIndices[planetCoordsIndicesIndex++]=(*it)->getA()->getid();
		planetCoordsIndices[planetCoordsIndicesIndex++]=(*it)->getB()->getid();
	}

	//the VBO for the planet coords
	glBindBuffer(GL_ARRAY_BUFFER, planetVBO);
	glBufferData(GL_ARRAY_BUFFER, planetCount*sizeof(GLfloat)*7, planetCoords, GL_STREAM_DRAW);
		GLERR("glBufferData");

	//the shader program to draw planets with
	glUseProgram(planetProgram);

	{
		matrix = glGetUniformLocation(planetProgram, "matrix");
		//temporary identity matrix
		__GetTempIdent;
		//turn it into a translation to this system's centre
		tempIdent[3]=centre[0];
		tempIdent[7]=centre[1];
		tempIdent[11]=centre[2];

		//copy the global rotation matrix into a local one
		memcpy(tempGlobal, globalMatrix, 16 * sizeof(GLfloat));

		//multiply this system's translation by the global rotation
		//and pass the result into the shader
		multMatrices4x4(tempIdent, tempGlobal);
		glUniformMatrix4fv(matrix, 1, GL_TRUE, tempGlobal);
			GLERR("pass in matrix");
	}

	//planet texture
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
		GLERR("Use lineProgram");

	{
		matrix = glGetUniformLocation(lineProgram, "matrix");
		glUniformMatrix4fv(matrix, 1, GL_TRUE, tempGlobal);
			GLERR("pass in matrix");
	}
	//vertex position
	position = glGetAttribLocation(lineProgram, "position");
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*7, 0);
	glEnableVertexAttribArray(position);

	//link colour
	colour = glGetUniformLocation(lineProgram, "data");
		GLERR("getting links ready to draw");

	glUniform3f(colour, 0.0f, 0.5f, 0.0f);

	//draw the links
	glDrawElements(GL_LINES, planetLinkTotal*2, GL_UNSIGNED_INT, planetCoordsIndices);

	glDisableVertexAttribArray(position);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
		GLERR("Drawing lines");

	delete planetCoords;
	delete planetCoordsIndices;
}

int System::getPlanetCount()
{
	return planetCount;
}

GLfloat System::getX()
{
	return centre[0];
}

GLfloat System::getY()
{
	return centre[1];
}

GLfloat System::getZ()
{
	return centre[2];
}

char* System::getName()
{
	return name;
}
