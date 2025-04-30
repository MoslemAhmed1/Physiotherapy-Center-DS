#pragma once
#include "LinkedQueue.h"

class EUWaitingList;
class XWaitingList;

#include "ArrayStack.h"

#include "priQueue.h"
class EarlyPList;

#include "ElectroResource.h"
#include "UltrasoundResourse.h"
#include "GymResource.h"

#include "Treatment.h"

#include "Definitions.h"
#include "Facilities.h"
#include "UI.h"
#include <fstream>
class Patient;

class Scheduler
{
private:

	// Available Resources Lists
	LinkedQueue<Resource*> availableResourcesE, availableResourcesU, availableResourcesX;

	// Waiting Lists
	EUWaitingList U_Waiting, E_Waiting;
	XWaitingList X_Waiting;

	// Patient Lists
	LinkedQueue<Patient*> allPatients;
	EarlyPList earlyPatients;
	priQueue<Patient*> latePatients, inTreatmentPatients;
	ArrayStack<Patient*> finishedPatients;

	ifstream inFile;
	ofstream outFile;
	UI* pUI;

	OPERATION_MODE opMode;

	int currentTimestep, allPatientsCount;
	int pCancel, pResc; // Cancellation and Reschedule Probabilities


public:

	// This Class should have member functions to :
	//	1- At program startup, open the input file and load patients to the "ALL" list
	// 
	//	2- A main Simulation function that at each timestep : 
	//		2.1- Checks "ALL" list and if a patient has arrived, move him to Early/Late/Wait
	//		2.2- Functions AddToWait_U to add a patient to U-Wait list and similar functions 
	//			to add to E-Wait and X-Wait. These functions are to be called by Treatment::MoveToWait function
	//		2.3- Assign patients to resoures
	//		2.4- When a patient is done with a treatment move him to Wait/Finish list
	//		2.5- Collect statistics that are needed to create output file
	//		2.6- Call UI class functions to print details on the output screen
	//	3- Produce the output file at the end of simulation

	Scheduler();

	void Simulate();

	void add_E_waiting(Patient* waitPatient);

	void add_U_waiting(Patient* waitPatient);

	void add_X_waiting(Patient* waitPatient);

	bool loadInputFile();

	void loadDevices();

	void assignProbabilities();

	void loadPatients();

	~Scheduler();
};


/*
void RandomWaiting(Patient* nextPatient)
	{
		nextPatient->setStatus(WAIT);
		int randomNumber = Facilities::generateRandomNumber(0, 100);

		if (randomNumber < 33) E_Waiting.enqueue_Latency(nextPatient); // here will be the Actual time of treatment
		else if (randomNumber < 66) U_Waiting.enqueue_Latency(nextPatient);
		else X_Waiting.enqueue_Latency(nextPatient);
	}
*/

/*

	bool Simulate()
	{
		// Calls UI method that takes the input file name from the user
		pUI->printWelcomeMessage();
		bool opened = loadInputFile();
		while (!opened) {
			pUI->printErrorMsg(FILE_NOT_OPEN);
			opened = loadInputFile();
		}
		while (finishedPatients.getCount() != allPatientsCount)
		{

			// 1- Check the allPatients list , if patient(s) arrive at the current timestep
			//		 move them to either (Early / Late / random Waiting) List

			Patient* nextPatient = nullptr;
			while (allPatients.peek(nextPatient))
			{
				if (nextPatient->getVT() == currentTimestep)
				{
					nextPatient->setStatus();
					PATIENT_STATUS status = nextPatient->getStatus();
					int PT = nextPatient->getPT();
					int VT = nextPatient->getVT();
					allPatients.dequeue(nextPatient);
					if (status == EARLY)
					{
						earlyPatients.enqueue(nextPatient, -PT);
					}
					else if (status == LATE)
					{
						int penalty = (VT - PT) / 2;
						latePatients.enqueue(nextPatient, -(VT + penalty));
					}
					else if (status == WAIT)
					{
						nextPatient->setPriority(nextPatient->getPT());
						RandomWaiting(nextPatient);
					}
				}
				else break;
			}

			// 2- Generate a random number X from 0 to 100 then:
			int randomNumber = Facilities::generateRandomNumber(0, 100);
			int pri;
			nextPatient = nullptr;

			// i. If randomNumber<10 , move next patient from Early to a RandomWaiting list
			if (randomNumber < 10)
			{
				if (earlyPatients.dequeue(nextPatient, pri))
				{
					nextPatient->setPriority(nextPatient->getPT());
					RandomWaiting(nextPatient);
				}
			}

			// ii. If 10<=randomNumber<20 , move next patient from Late to a RandomWaiting list and insert him using PT+penalty time.
			if (randomNumber >= 10 && randomNumber < 20)
			{
				if (latePatients.dequeue(nextPatient, pri)) // Special case so i didn't call RandomWaiting
				{
					nextPatient->setStatus(WAIT);
					int priority = ((nextPatient->getVT() - nextPatient->getPT()) / 2) + nextPatient->getPT();
					nextPatient->setPriority(priority);
					int randomN = Facilities::generateRandomNumber(0, 100);
					if (randomN < 33) E_Waiting.insertSorted(nextPatient);
					else if (randomN < 66) U_Waiting.insertSorted(nextPatient);
					else X_Waiting.insertSorted(nextPatient);
				}
			}

			// iii. If 20<=randomNumber<40 , move 2 next patients from a RandomWaiting to In-treatment list (QUESTION!!)
			if (randomNumber >= 20 && randomNumber < 40)
			{



				int randomN = Facilities::generateRandomNumber(0, 100);
				if (randomN < 33)
				{
					for (int i = 0; i < 2; i++)
						if (E_Waiting.dequeue_Latency(nextPatient))
						{
							nextPatient->setStatus(SERV);
							Resource* eResource;
							availableResourcesE.peek(eResource);
							bool treatmentExists = nextPatient->getCurrTreatment();
							if (treatmentExists)
								nextPatient->getCurrTreatment()->setResource(eResource);
							nextPatient->setPriority(currentTimestep + nextPatient->getCurrentDuration());
							inTreatmentPatients.enqueue(nextPatient, -nextPatient->getPriority());
						}
				}
				else if (randomN < 66)
				{
					for (int i = 0; i < 2; i++)
						if (U_Waiting.dequeue_Latency(nextPatient))
						{
							Resource* uResource;
							availableResourcesU.peek(uResource);
							bool treatmentExists = nextPatient->getCurrTreatment();
							if (treatmentExists)
								nextPatient->getCurrTreatment()->setResource(uResource);
							nextPatient->setStatus(SERV);
							nextPatient->setPriority(currentTimestep + nextPatient->getCurrentDuration());
							inTreatmentPatients.enqueue(nextPatient, -nextPatient->getPriority());
						}
				}
				else
				{
					for (int i = 0; i < 2; i++)
						if (X_Waiting.dequeue_Latency(nextPatient))
						{
							Resource* xResource;
							availableResourcesX.peek(xResource);
							bool treatmentExists = nextPatient->getCurrTreatment();
							if (treatmentExists)
								nextPatient->getCurrTreatment()->setResource(xResource);
							nextPatient->setStatus(SERV);
							nextPatient->setPriority(currentTimestep + nextPatient->getCurrentDuration());
							inTreatmentPatients.enqueue(nextPatient, -nextPatient->getPriority());
						}
				}
			}

			// iv. If 40<=randomNumber<50 , move next patient from In-treatment to a RandomWaiting list
			if (randomNumber >= 40 && randomNumber < 50)
			{
				if (inTreatmentPatients.dequeue(nextPatient, pri))
				{
					nextPatient->setPriority(nextPatient->getPT());
					RandomWaiting(nextPatient);
				}
			}

			// v. If 50<=randomNumber<60 , move next patient from In-treatment to Finish
			if (randomNumber >= 50 && randomNumber < 60)
			{
				if (inTreatmentPatients.dequeue(nextPatient, pri))
				{
					nextPatient->setStatus(FNSH);
					nextPatient->setFT(currentTimestep);
					nextPatient->setTW();
					finishedPatients.push(nextPatient);
				}
			}

			// vi. If 60<=X<70 , move random patient from X-Waiting to Finish, to simulate the cancel process
			if (randomNumber >= 60 && randomNumber < 70)
			{
				Patient* patient = nullptr;
				bool cancelled = 0;
				for (int i = 0; i < 30 && !cancelled; i++)
				{
					if (X_Waiting.cancel(patient))
					{
						patient->setFT(currentTimestep);
						patient->setTW();
						finishedPatients.push(patient);
						cancelled = true;
					}
				}
			}

			// vii. If 70<=X<80 , choose random patient from Early list to the appropriate list to perform an accepted reschedule process
			if (randomNumber >= 70 && randomNumber < 80)
			{
				if (!earlyPatients.isEmpty())
				{
					earlyPatients.reschedule();
				}
			}

			// 4- Print all applicable info to console as in the "Program Interface" section
			pUI->printMaster(
				currentTimestep, &availableResourcesE, &availableResourcesU, &availableResourcesX,
				&U_Waiting, &E_Waiting, &X_Waiting, &allPatients, &earlyPatients, &finishedPatients,
				&latePatients, &inTreatmentPatients
			);

			bool proceed = pUI->proceed();
			if (!proceed)
				break;

			currentTimestep++;
		}

		return true;

	}

*/