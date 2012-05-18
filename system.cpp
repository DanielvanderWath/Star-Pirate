#include "system.h"

bool moveAway(Planet *A, Planet *B, float tolerance)
{
	//move two planets away from each other if they are closer than tolerance
	float Ax = A->getX(), Ay = A->getY();
	float Bx = B->getX(), By = B->getY();
	float Dx = Ax - Bx, Dy = Ay - By;
	
	//the float version of abs wasn't being pulled in, grr
	if(Dx < 0.0f)
		Dx=-Dx;
	if(Dy < 0.0f)
		Dy=-Dy;

	float distance = sqrt(Dx*Dx + Dy*Dy);

	if(distance == 0.0f)
	{
		//if they are in the same place, move A a little to the left and let the next pass sort them out. TODO: move them in random directions
//printf("distance %f, A=%d B=%d\n", distance, A->getid(), B->getid());
		float push[3]={tolerance/2.0f, tolerance/2.0f, 0.0f};
		if(A->move(push))
		{
			//destroy the planet, we've probably locked up
			A->markForDestruction();
		}

		/*push[0] *= -1.0f;
		push[1] *= -1.0f;

		B->move(push);
		*/return true;
	}
	else if(distance < tolerance)
	{
		float push[3]={ Ax < Bx ? (Bx-Ax)/2.0f : (Ax-Bx)/2.0f, Ay < By ? (By-Ay)/2.0f : (Ay-By)/2.0f, 0.0f};

		if(B->move(push))
		{
			//destroy the planet, we've probably locked up
			B->markForDestruction();
		}

/*		push[0]=-push[0];
		push[1]=-push[1];

		A->move(push);
*/
		B->print();
		B->putBackInBounds(-1.0f, 1.0f, -1.0f, 1.0f);
		B->print();

		return true;
	}
	return false;
}

System::System(const char *Name, int numPlanets, float *syscentre)
{
	planetCount=0;
	nextPlanetId=0;
	planetLinkTotal=0;

	static int systemCount=0;

	id=systemCount++;

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
		Planet *p = new Planet(rand() % NUM_PLANET_TYPES, location, &nextPlanetId);
		planetCount++;
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

System::~System()
{
	for(list<Planet*>::iterator it=planetList.begin(); it!=planetList.end(); it++)
	{
		planetList.erase(it);
		delete (*it);
	}
}

void System::unOverLap()
{
	bool finished = false;
	while(!finished)
	{
		finished = true;
		//make sure none of them overlap
		for(list<Planet*>::iterator it=planetList.begin(); it!=planetList.end(); it++)
		{
			//set to false if we find an overlapping pair
			for(list<Planet*>::iterator jt=it; jt!=planetList.end(); jt++)
			{
				if((*it)->getid()!=(*jt)->getid())
					if(moveAway((*it), (*jt), 0.2f))
						finished=false;

				if((*jt)->getKillMe())
				{
					//destroy this planet, it's causing
					//trouble
					
					//first remove all of its links
					for(list<Link*>::iterator lit=links.begin(); lit != links.end(); lit++)
					{
						if((*lit)->getA() == (*jt) || (*lit)->getB() == (*jt))
						{
							list<Link*>::iterator tempLit=lit;
							lit=links.erase(lit);
							delete (*tempLit);
							planetLinkTotal--;
						}
					}

					//decrement the coord indices of all list items after this one
					for(list<Planet*>::iterator kt=jt; kt != planetList.end(); kt++)
						(*kt)->decrementCoordIndex();
					//then get rid of this planet
					planetList.erase(jt);
					delete (*jt);
					planetCount--;
					break;
				}
			}
		}
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
		GLuint planetType=(*it)->getType();
		GLfloat coords[]={(*it)->getX(),(*it)->getY(),(*it)->getZ()};

		//copy planet coords to planet coords array
		planetCoords[planetCoordsIndex++]=coords[0]/SYSTEM_SCALE;
		planetCoords[planetCoordsIndex++]=coords[1]/SYSTEM_SCALE;
		planetCoords[planetCoordsIndex++]=coords[2]/SYSTEM_SCALE;

		//copy planet type (texture) to planet coords array
		planetCoords[planetCoordsIndex++]=(GLfloat)planetType;

	}

	//get the links ready
	for(list<Link*>::iterator it=links.begin(); it!=links.end(); it++)
	{
		planetCoordsIndices[planetCoordsIndicesIndex++]=(*it)->getA()->getCoordIndex();
		planetCoordsIndices[planetCoordsIndicesIndex++]=(*it)->getB()->getCoordIndex();
	}

	//the VBO for the planet coords
	glBindBuffer(GL_ARRAY_BUFFER, planetVBO);
	glBufferData(GL_ARRAY_BUFFER, planetCount*sizeof(GLfloat)*4, planetCoords, GL_STREAM_DRAW);
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

	//this should really go somewhere else, in fact the whole way I'm doing these textures is horrible
	GLint planetTypesArray[]={
				PLANET_TYPE_EARTH,
				PLANET_TYPE_LAVA,
				PLANET_TYPE_ICE,
				PLANET_TYPE_DESERT,
				};

	//planet textures
	texture = glGetUniformLocation(planetProgram, "tex");
	glUniform1iv(texture, NUM_PLANET_TYPES, planetTypesArray);
		GLERR("pass in texture");

	//vertex colour
	colour = glGetAttribLocation(planetProgram, "texIndex");
	glVertexAttribPointer(colour, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)(sizeof(GLfloat)*3));
		GLERR("data attrib");

	//vertex position
	position = glGetAttribLocation(planetProgram, "position");
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, 0);
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
	glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, 0);
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

void System::move(float *vector)
{
	for(int i=0; i<3; i++)
		centre[i]+=vector[i];
}

char* System::getName()
{
	return name;
}

int System::getid()
{
	return id;
}
