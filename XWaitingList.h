#pragma once
#include "Patient.h"
#include "EUWaitingList.h"
#include "Facilities.h"


class XWaitingList : public EUWaitingList
{

public:
    
    XWaitingList() :EUWaitingList()
    {
    }

    bool cancel(Patient*& patient)
    {
        if (isEmpty())
            return false;

        int randomPosition = Facilities::generateRandomNumber(1, count);
        Node<Patient*>* prevPtr = nullptr;
        Node<Patient*>* currentPtr = frontPtr;

        for (int i = 1; i < randomPosition && currentPtr != nullptr; i++)
        {
            prevPtr = currentPtr;
            currentPtr = currentPtr->getNext();
        }

        Patient* patientPtr = currentPtr->getItem();

        if (patientPtr->lastTreatment())
        {
            patientPtr->setStatus(FNSH);
            patientPtr->cancel(); // sets fCancelled to true

            if (frontPtr == backPtr) // The list has only one patient
            {
                frontPtr = backPtr = nullptr;
                currentPtr->setNext(nullptr); 
                delete currentPtr;
            }

            else if (prevPtr == nullptr)    // The patient is at the head
            {
                frontPtr = currentPtr->getNext();
                currentPtr->setNext(nullptr); 
                delete currentPtr;

            }
            else if (currentPtr->getNext() == nullptr) // The patient is at the end
            {
                backPtr = prevPtr;
                prevPtr->setNext(currentPtr->getNext());
                currentPtr->setNext(nullptr); 
                delete currentPtr;
            }
            else // The patient is somewhere in the middle
            {
                prevPtr->setNext(currentPtr->getNext());
                currentPtr->setNext(nullptr); 
                delete currentPtr;
            }

            treatmentLatency -= patientPtr->getCurrentDuration();
            patient = patientPtr;
            // clear the pointer to avoid dangling reference
            patientPtr = nullptr;
            prevPtr = nullptr;
            currentPtr = nullptr;

            count--;
            return true;
        }
        else
            return false;
    }

    ~XWaitingList()
    {

    }
};

