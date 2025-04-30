#pragma once
#include "Treatment.h"

class ElectroTreatment : public Treatment
{
private:
	int static number;

public:
	ElectroTreatment(int time) : Treatment(time)
	{
	}

	bool CanAssign(Resource* resource)
	{
		return (resource != nullptr);
	}

	/*
	bool MoveToWait(Scheduler* scheduler, Patient* patient)
	{
		return false; // To be changed
	}
	*/

	void PrintInfo(ostream& os) const
	{
		// Will be used in phase 2
		//os << 'E' << resourcePtr->getRID();
	}

	~ElectroTreatment() {}
};

int ElectroTreatment::number = 1;