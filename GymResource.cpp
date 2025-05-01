#include "GymResource.h"

GymResource::GymResource(int cap) : Resource()
{
	currentPatients = 0;
	capacity = cap;
	RID = number;
	RType = GYM;
	number++;
}

void GymResource::incrementCurrPatients(int num)
{
	currentPatients += num;
}

int GymResource::getCurrPatients() const
{
	return currentPatients;
}

int GymResource::getCapacity() const
{
	return capacity;
}

void GymResource::PrintInfo(ostream& os) const
{
	os << "X" << RID << "[" << currentPatients << ',' << capacity << "]";
}

GymResource::~GymResource() { number--; }

int GymResource::number = 1;
