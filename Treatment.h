#pragma once
#include "Resource.h"

class Patient;
class Scheduler;

class Treatment
{
protected:

	int duration, assignmentTime;
	Resource* resourcePtr;

public:

	Treatment(int time) : duration(time)
	{
		assignmentTime = 0;
		resourcePtr = nullptr;
	}

	void setResource(Resource* r) 
	{
		resourcePtr = r;
	}

	Resource* getResource() const
	{
		return resourcePtr;
	}

	virtual bool CanAssign(Resource* recource) = 0;

	virtual bool MoveToWait(Scheduler* scheduler, Patient* patient) = 0;	// Moves the patient to the appropriate waiting list. It should call
							
	virtual TREATMENT_TYPE getType() = 0;
	
	// the appropriate "AddToWait" function from the 
	// Will be used in phase 2
	//virtual void PrintInfo(ostream& os) const = 0;

	friend ostream& operator << (ostream& os, const Treatment& treat)
	{
		//treat.PrintInfo(os);
		switch (treat.resourcePtr->getType())
		{
		case ULTRASOUND:
			os << 'U';
			break;
		case ELECTRO:
			os << 'E';
			break;
		case GYM:
			os << 'G';
		}
		os << treat.resourcePtr->getRID();

		return os;
	}

	int getDuration() { return duration; }

	void setAssignmentTime(int time) { assignmentTime = time; }
	int getAssignmentTime() { return assignmentTime; }



	~Treatment()
	{
	}
};

