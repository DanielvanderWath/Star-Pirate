#include "link.h"

Link::Link(Planet *a, Planet *b)
{
	A=a;
	B=b;
}

Link::~Link()
{
}

void Link::print()
{
	printf("\t%d---->%d\n", A->getid(), B->getid());
}

Planet* Link::getA()
{
	return A;
}

Planet* Link::getB()
{
	return B;
}

