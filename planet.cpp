#include "planet.h"
#include <stdio.h>

using namespace std;

static int numPlanets=0;

Planet::Planet(unsigned int Colour, float *location, int *planetCount)
{
	id=(*planetCount)++;

	printf("Creating planet\n\tid=%d\n\tcolour=0x%.8x\n\tcoords=%f %f %f\n", id, Colour, location[0], location[1], location[2]);


	planetColour=Colour;

	for(int i=0; i<3; i++)
		coords[i]=location[i];
}

int Planet::getid()
{
	return id;
}

unsigned int Planet::getColour()
{
	return planetColour;
}

GLfloat Planet::getX()
{
	return coords[0];
}

GLfloat Planet::getY()
{
	return coords[1];
}

GLfloat Planet::getZ()
{
	return coords[2];
}

void Planet::move(float *vector)
{
	for(int i = 0; i<3 ; i++)
		coords[i]+=vector[i];
}
