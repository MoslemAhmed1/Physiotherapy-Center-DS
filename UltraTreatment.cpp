#include "UltraTreatment.h"
#include "Scheduler.h"

UltraTreatment::UltraTreatment(int time):Treatment(time)
{
}

TREATMENT_TYPE UltraTreatment::getType()
{
	return ULTRATREATMENT;
}

bool UltraTreatment::CanAssign(Resource* recource)
{
	return (recource != nullptr);
}

bool UltraTreatment::MoveToWait(Scheduler* scheduler, Patient* waitPatient)
{
	if (waitPatient && scheduler)
	{
		scheduler->add_U_waiting(waitPatient);
		return true;
	}
	return false;
}

UltraTreatment::~UltraTreatment()
{
}

int UltraTreatment::number = 1;
