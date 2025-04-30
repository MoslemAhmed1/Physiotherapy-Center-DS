#pragma once
#include "Resource.h"

class GymResource : public Resource
{
	static int number;
	int capacity, currentPatients;

public:

	GymResource(int cap);

	void setCurrPatients(int num);

	int getCurrPatients() const;

	void PrintInfo(ostream& os) const;

	~GymResource();



};

