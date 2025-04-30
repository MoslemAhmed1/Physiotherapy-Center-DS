#pragma once
#include "Treatment.h"

class UltraTreatment : public Treatment
{
private:
	int static number;

public:
	UltraTreatment(int time) : Treatment(time)
	{
	}

	bool CanAssign(Resource* recource)
	{
		return (recource != nullptr);
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
		//os << 'U' << resourcePtr->getRID();
	}

	~UltraTreatment() {}
};

int UltraTreatment::number = 1;
