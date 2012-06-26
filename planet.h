#if !defined(__PLANET_H__)
#define __PLANET_H__
#include "starpirate.h"
#include <list>

using namespace std;

class Planet
{
	private:
		unsigned int planetType;
		int id, coordIndex; //coordIndex is used to index the vertices later, it can't have gaps like the IDs can. It will be rejiggered when planets are deleted/added
		float coords[3];
		bool killme;
		int timesMoved;
		unsigned int resource_metal;
		unsigned int resource_crystal;
		unsigned int resource_food;
		unsigned int resource_silicon;
	public:
		Planet(unsigned int Colour, float *location, int *nextPlanetId);
		~Planet();
		int getid();
		int getCoordIndex();
		void decrementCoordIndex();
		unsigned int getType();
		GLfloat getX();
		GLfloat getY();
		GLfloat getZ();
		bool move(float *vector);
		void putBackInBounds(float xlow, float xhigh, float ylow, float yhigh);
		void print();
		void markForDestruction();
		bool getKillMe();
		void initResources();
		void printResources();
};
#endif

