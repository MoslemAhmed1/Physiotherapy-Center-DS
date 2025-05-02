#include "GymTreatment.h"
#include "Scheduler.h"

GymTreatment::GymTreatment(int time) : Treatment(time) {}

TREATMENT_TYPE GymTreatment::getType() {
    return GYMTREATMENT;
}

bool GymTreatment::CanAssign(Resource* resource) 
{
	GymResource* gymResource = dynamic_cast<GymResource*>(resource);
    bool canAssign = false;
	if (gymResource && gymResource->getCurrPatients() < gymResource->getCapacity()) 
    {
		canAssign = true;
	}
    return canAssign;
}

void GymTreatment::setResource(Resource* r) 
{
	GymResource* rPtr = dynamic_cast<GymResource*>(r);
	if (rPtr)
	{
		rPtr->incrementCurrPatients(1);
	}
	resourcePtr = rPtr;
}

void GymTreatment::removeResource()
{
	GymResource* rPtr = dynamic_cast<GymResource*>(resourcePtr);
	if (rPtr)
	{
		rPtr->incrementCurrPatients(-1);
	}
	resourcePtr = nullptr;
}

bool GymTreatment::MoveToWait(Scheduler* scheduler, Patient* waitPatient) {
    if (waitPatient && scheduler) {
        scheduler->AddToWait_X(waitPatient);
        return true;
    }
    return false;
}

GymTreatment::~GymTreatment()
{
}

int GymTreatment::number = 1;
