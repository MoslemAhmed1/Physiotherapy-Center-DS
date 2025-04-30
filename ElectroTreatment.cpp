#include "ElectroTreatment.h"
#include "Scheduler.h"

int ElectroTreatment::number = 1;  // Define static variable ONLY HERE

ElectroTreatment::ElectroTreatment(int time) : Treatment(time) {}

TREATMENT_TYPE ElectroTreatment::getType() {
    return ELECTROTREATMENT;
}

bool ElectroTreatment::CanAssign(Resource* resource) {
    return (resource != nullptr);
}

bool ElectroTreatment::MoveToWait(Scheduler* scheduler, Patient* waitPatient) {
    if (waitPatient && scheduler) 
    {
        scheduler->add_E_waiting(waitPatient);
        return true;
    }
    return false;
}

ElectroTreatment::~ElectroTreatment() {}
