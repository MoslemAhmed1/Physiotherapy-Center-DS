#pragma once
#include "LinkedQueue.h"
#include "Patient.h"

class EUWaitingList : public LinkedQueue<Patient*>
{
protected:
    int treatmentLatency; // Treatment latency for the patients in the waiting list

public:

    EUWaitingList();

    bool insertSorted(Patient* const& patient);	/*  Insert a patient in the waiting list sorted by either PT + penalty for late patient,
                                                or PT for patient who finished a treatment */
    virtual void PrintInfo();

    bool enqueue_Latency(Patient*& newEntry);

    bool dequeue_Latency(Patient*& patient);


    ~EUWaitingList();

};

EUWaitingList::EUWaitingList() :LinkedQueue<Patient*>()
{
    treatmentLatency = 0;
}

bool EUWaitingList::enqueue_Latency(Patient*& patient)
{
    treatmentLatency += patient->getCurrentDuration();
    return enqueue(patient);
}

bool EUWaitingList::dequeue_Latency(Patient*& patient)
{
    bool removed = dequeue(patient);
    if (removed)
        treatmentLatency -= patient->getCurrentDuration();  // Changed

    return removed;
}

bool EUWaitingList::insertSorted(Patient* const& patient)
{
    treatmentLatency += patient->getCurrentDuration();
    // 1- If Empty Queue -> enqueue
    if (isEmpty())
    {
        enqueue(patient);
        return true;
    }

    Node<Patient*>* newPatient = new Node<Patient*>(patient);
    if (patient->getPriority() < frontPtr->getItem()->getPriority())
    {
        newPatient->setNext(frontPtr);
        frontPtr = newPatient;
        count++;
        return true;
    }

    Node<Patient*>* ptr = frontPtr;

    while (ptr->getNext())
    {
        if (ptr->getNext()->getItem()->getPriority() > patient->getPriority())
            break;
        ptr = ptr->getNext();
    }

    newPatient->setNext(ptr->getNext());
    ptr->setNext(newPatient);
    if (ptr == backPtr)
        backPtr = newPatient;

    count++;
    return true;
}

void EUWaitingList::PrintInfo()
{
    //Use pointer manipulation to print the queue
    if (isEmpty())
    {
        cout << endl;
        return;
    }

    Node<Patient*>* traverse = frontPtr;

    while (traverse->getNext())
    {
        cout << *(traverse->getItem()) << ", ";
        traverse = traverse->getNext();
    }

    cout << *(traverse->getItem()) << endl;

    traverse = nullptr;
}

EUWaitingList::~EUWaitingList()
{
}