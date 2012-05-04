#if !defined(__PLANET_H__)
#define __PLANET_H__
#include "starpirate.h"
#include <list>

using namespace std;

class Planet
{
	private:
		unsigned int planetColour;
		int id, coordIndex; //coordIndex is used to index the vertices later, it can't have gaps like the IDs can. It will be rejiggered when planets are deleted/added
		float coords[3];
		bool killme;
		int timesMoved;
	public:
		Planet(unsigned int Colour, float *location, int *nextPlanetId);
		~Planet();
		int getid();
		int getCoordIndex();
		void decrementCoordIndex();
		unsigned int getColour();
		GLfloat getX();
		GLfloat getY();
		GLfloat getZ();
		bool move(float *vector);
		void putBackInBounds(float xlow, float xhigh, float ylow, float yhigh);
		void print();
		void markForDestruction();
		bool getKillMe();
};
#endif

