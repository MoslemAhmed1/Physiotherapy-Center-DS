#pragma once
#include "Treatment.h"


class GymTreatment : public Treatment
{
private:
	int static number;

public:
	GymTreatment(int time);

	TREATMENT_TYPE getType();


	bool CanAssign(Resource* resource);


	bool MoveToWait(Scheduler* scheduler, Patient* patient);


	~GymTreatment();
};


