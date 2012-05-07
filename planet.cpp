#include "planet.h"
#include <stdio.h>

using namespace std;

static int numPlanets=0;

Planet::Planet(unsigned int Colour, float *location, int *nextPlanetId)
{
	id=(*nextPlanetId)++;
	coordIndex=id;

	printf("Creating planet\n\tid=%d\n\tcolour=0x%.8x\n\tcoords=%f %f %f\n", id, Colour, location[0], location[1], location[2]);

	planetType=Colour;

	killme=false;
	timesMoved=0;

	for(int i=0; i<3; i++)
		coords[i]=location[i];
}

Planet::~Planet()
{
	printf("deleting planet\n");
}

int Planet::getCoordIndex()
{
	return coordIndex;
}

void Planet::decrementCoordIndex()
{
	if(coordIndex<1)
		return;//should never happen, but just in case do nothing
	coordIndex--;
}

int Planet::getid()
{
	return id;
}

unsigned int Planet::getType()
{
	return planetType;
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

bool Planet::move(float *vector)
{
	//to avoid locking up when trying to unoverlap systems
	//just give up and destroy the planet after a certain number of tries
	if(timesMoved++ == MAX_PLANET_MOVES_BEFORE_GIVE_UP)
		return true;
		
	for(int i = 0; i<3 ; i++)
		coords[i]+=vector[i];

	return false;
}

void Planet::putBackInBounds(float xlow, float xhigh, float ylow, float yhigh)
{
	if(coords[0] < xlow)
		coords[0]=xhigh - (xlow - coords[0]);
	else if(coords[0] > xhigh)
		coords[0]=xlow + (coords[0] - xhigh);

	if(coords[0] < ylow)
		coords[0]=yhigh - (ylow - coords[0]);
	else if(coords[0] > yhigh)
		coords[0]=ylow + (coords[0] - yhigh);

}

void Planet::print()
{
	printf("planet %d at %f %f\n", id, coords[0], coords[1]);
}

void Planet::markForDestruction()
{
	killme=true;
}

bool Planet::getKillMe()
{
	return killme;
}

