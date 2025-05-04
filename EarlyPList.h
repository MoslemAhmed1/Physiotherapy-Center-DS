#pragma once
#include "priQueue.h"
#include "Patient.h"
#include"Facilities.h"

class EarlyPList : public priQueue<Patient*>
{
public:

    EarlyPList() : priQueue()
    {
    }

    bool reschedule()
    {
        // Return false if empty list
        if (isEmpty()) 
            return false;

        // Choose a random patient position (1 to count)
        int randomPosition = Facilities::generateRandomNumber(1, count);

        // Find the randomly selected patient
        priNode<Patient*>* prevPtr = nullptr;
        priNode<Patient*>* currentPtr = head;

        // Search for the randomly selected patient
        for (int i = 1; i < randomPosition && currentPtr != nullptr; i++)
        {
            prevPtr = currentPtr;
            currentPtr = currentPtr->getNext();
        }

        // Remove the patient from the list
        if (prevPtr == nullptr)
            head = currentPtr->getNext(); // The patient is at the head
        else
            prevPtr->setNext(currentPtr->getNext()); // The patient is somewhere in the middle or end

        currentPtr->setNext(nullptr);
        count--;

        // Calculate new PT (higher than old PT)

        int oldPT = 0;
        Patient* patient = currentPtr->getItem(oldPT);
        oldPT = patient->getPT();


        int newPT = oldPT;
        rand();
        if (patient->getNumReschedules() <= MAX_RESCHEDULES) {
            newPT = Facilities::generateRandomNumber(oldPT, oldPT + MAX_RESCHEDULE_TIME);
            patient->reschedule(); // Adds the number of reschedules
        }
        patient->setPT(newPT);
        // Re-enqueue the patient with the new priority
        enqueue(patient, -newPT);

        return true;
    }

    ~EarlyPList()
    {
    }

    
};

