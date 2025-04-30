#pragma once
#include "Resource.h"

class GymResource : public Resource
{
	static int number;
	int capacity, currentPatients;

public:

	GymResource(int cap)
	{
		currentPatients = 0;
		capacity = cap;
		RID = number;
		RType = GYM;
		number++;
	}

	void setCurrPatients(int num)
	{
		currentPatients = num;
	}

	int getCurrPatients() const
	{
		return currentPatients;
	}

	void PrintInfo(ostream& os) const override
	{
		os << "X" << RID << "[" << currentPatients << ',' << capacity << "]";
	}

	~GymResource() { number--; }



};

int GymResource::number = 1;

