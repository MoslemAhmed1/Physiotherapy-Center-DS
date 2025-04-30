#pragma once
#include "Treatment.h"

class Patient;
class Scheduler;

class ElectroTreatment : public Treatment {
private:
    static int number;  // Declaration only

public:
    ElectroTreatment(int time);
    TREATMENT_TYPE getType();
    bool CanAssign(Resource* resource);
    //bool MoveToWait(Scheduler* scheduler, Patient* patient);
    ~ElectroTreatment();
};
