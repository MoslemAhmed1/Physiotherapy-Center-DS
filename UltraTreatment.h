#pragma once
#include "Treatment.h"


class UltraTreatment : public Treatment
{
private:
	int static number;

public:
	UltraTreatment(int time);


	TREATMENT_TYPE getType();


	bool CanAssign(Resource* recource);


	bool MoveToWait(Scheduler* scheduler, Patient* patient);




	~UltraTreatment();
};

