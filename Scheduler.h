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

	int totalPenaltyTime;
	int earlyNum, lateNum;

public:

	// This Class should have member functions to :
	//	- At program startup, open the input file and load patients to the "ALL" list (DONE)
	// 
	//	- A main Simulation function that at each timestep : 
	//		1- Checks "ALL" list and if a patient has arrived, move him to Early/Late/Wait (DONE)
	//		2- Functions AddToWait_U to add a patient to U-Wait list and similar functions (DONE)
	//			to add to E-Wait and X-Wait. These functions are to be called by Treatment::MoveToWait function
	//		3- Assign patients to resoures (DONE)
	//		4- When a patient is done with a treatment move him to Wait/Finish list (DONE)
	//		5- Collect statistics that are needed to create output file (DONE)
	//		6- Call UI class functions to print details on the output screen (DONE)
	//	- Produce the output file at the end of simulation (DONE)

	Scheduler();

	void Simulate();

	void AddToWait_E(Patient* waitPatient);

	void AddToWait_U(Patient* waitPatient);

	void AddToWait_X(Patient* waitPatient);

	bool loadInputFile();

	bool generateOutputFile();

	void loadDevices();

	void assignProbabilities();

	void loadPatients();

	~Scheduler();
};
