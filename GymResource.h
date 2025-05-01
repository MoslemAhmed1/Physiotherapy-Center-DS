#pragma once
#include "Resource.h"

class GymResource : public Resource
{
	static int number;
	int capacity, currentPatients;

public:

	GymResource(int cap);

	void incrementCurrPatients(int num);

	int getCurrPatients() const;

	int getCapacity() const;

	void PrintInfo(ostream& os) const;

	~GymResource();



};

