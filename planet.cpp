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

	initResources();
	printResources();

	killme=false;
	timesMoved=0;

	for(int i=0; i<3; i++)
		coords[i]=location[i];
}

Planet::~Planet()
{
	printf("deleting planet\n");
}

void Planet::printResources()
{
	printf("Resources:\n");
	if(resource_food != 0)
		printf("\tFood: %d\n", resource_food);
	if(resource_crystal != 0)
		printf("\tCrystal: %d\n", resource_crystal);
	if(resource_metal != 0)
		printf("\tMetal: %d\n", resource_metal);
	if(resource_silicon != 0)
		printf("\tSilicon: %d\n", resource_silicon);
}

void Planet::initResources()
{
	switch(planetType)
	{
		case PLANET_TYPE_EARTH:
			resource_food = (rand() % 800) + 200;
			resource_crystal = 0;
			resource_metal = 0;
			resource_silicon = 0;
			break;
		case PLANET_TYPE_LAVA:
			resource_food = 0;
			resource_crystal = 0;
			resource_metal = (rand() % 800) + 200;
			resource_silicon = 0;
			break;
		case PLANET_TYPE_ICE:
			resource_food = 0;
			resource_crystal = (rand() % 800) + 200;
			resource_metal = 0;
			resource_silicon = 0;
			break;
		case PLANET_TYPE_DESERT:
			resource_food = 0;
			resource_crystal = 0;
			resource_metal = 0;
			resource_silicon = (rand() % 800) + 200;
			break;
		default:
			printf("ERROR: Invalid planet type\n");
		
	}
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

