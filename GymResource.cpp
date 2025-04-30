#include "GymResource.h"

GymResource::GymResource(int cap) : Resource()
{
	currentPatients = 0;
	capacity = cap;
	RID = number;
	RType = GYM;
	number++;
}

void GymResource::setCurrPatients(int num)
{
	currentPatients = num;
}

int GymResource::getCurrPatients() const
{
	return currentPatients;
}

void GymResource::PrintInfo(ostream& os) const
{
	os << "X" << RID << "[" << currentPatients << ',' << capacity << "]";
}

GymResource::~GymResource() { number--; }

int GymResource::number = 1;
