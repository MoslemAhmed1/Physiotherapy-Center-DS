#pragma once
#include "Treatment.h"

class GymTreatment : public Treatment
{
private:
	int static number;

public:
	GymTreatment(int time) : Treatment(time)
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
		//os << 'G' << resourcePtr->getRID();
	}

	~GymTreatment() {}
};

int GymTreatment::number = 1;

