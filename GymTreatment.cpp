#include "GymTreatment.h"
#include "Scheduler.h"

GymTreatment::GymTreatment(int time) : Treatment(time) {}

TREATMENT_TYPE GymTreatment::getType() {
    return GYMTREATMENT;
}

bool GymTreatment::CanAssign(Resource* resource) {
    return (resource != nullptr);
}


//bool GymTreatment::MoveToWait(Scheduler* scheduler, Patient* waitPatient) {
//    if (waitPatient && scheduler) {
//        scheduler->add_X_waiting(waitPatient);
//        return true;
//    }
//    return false;
//}

GymTreatment::~GymTreatment()
{
}

int GymTreatment::number = 1;
