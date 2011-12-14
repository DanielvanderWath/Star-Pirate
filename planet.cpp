#include "planet.h"
#include <stdio.h>

using namespace std;

static int numPlanets=0;

Planet::Planet(unsigned int Colour, float *location)
{
	id=numPlanets++;

	printf("Creating planet\n\tid=%d\n\tcolour=0x%.8x\n\tcoords=%f %f %f\n", id, Colour, location[0], location[1], location[2]);


	planetColour=Colour;

	linkCount=0;

	for(int i=0; i<3; i++)
		coords[i]=location[i];
}

void Planet::addLink(Planet *link)
{
	links.push_back(link);
	linkCount++;
}

void Planet::printLinks()
{
	for(list<Planet*>::iterator it=links.begin(); it!=links.end(); it++)
		printf("\t%d---->%d\n", id, (*it)->getid());
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

int Planet::getLinkCount()
{
	return linkCount;
}

list<Planet*>::iterator Planet::getLinksBegin()
{
	return links.begin();
}

list<Planet*>::iterator Planet::getLinksEnd()
{
	return links.end();
}
