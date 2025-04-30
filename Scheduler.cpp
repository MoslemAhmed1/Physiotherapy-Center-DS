#include "Scheduler.h"
#include "EUWaitingList.h"
#include "XWaitingList.h"
#include "Patient.h"


Scheduler::Scheduler()
{
	pUI = new UI;
	currentTimestep = 1;
	pCancel = pResc = 0;
	allPatientsCount = 0;
}

void Scheduler::Simulate()
{
	pUI->printWelcomeMessage();
	bool opened = loadInputFile();
	while (!opened) {
		pUI->printErrorMsg(FILE_NOT_OPEN);
		opened = loadInputFile();
	}

	while (finishedPatients.getCount() != allPatientsCount)
	{	
		// 1- Check the allPatients list , if patient(s) arrive at the current timestep
		//	move them to either (Early / Late / random Waiting) List

		Patient* nextPatient = nullptr;
		while (allPatients.peek(nextPatient))
		{
			if (nextPatient->getVT() == currentTimestep)
			{
				allPatients.dequeue(nextPatient);
				nextPatient->setStatus();

				PATIENT_STATUS status = nextPatient->getStatus();
				int PT = nextPatient->getPT();
				int VT = nextPatient->getVT();

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
					int E_Latency = E_Waiting.getLatency();
					int U_Latency = U_Waiting.getLatency();
					int X_Latency = X_Waiting.getLatency();

					nextPatient->reorderTreatments(E_Latency, U_Latency, X_Latency);

					nextPatient->getCurrTreatment()->MoveToWait(this, nextPatient);
				}
			}
			else break;
		}

		nextPatient = nullptr;
		int leavingTime = 0;

		while (earlyPatients.peek(nextPatient, leavingTime))
		{
			leavingTime = -leavingTime;
			if (leavingTime == currentTimestep)
			{
				earlyPatients.dequeue(nextPatient, leavingTime);

				int E_Latency = E_Waiting.getLatency();
				int U_Latency = U_Waiting.getLatency();
				int X_Latency = X_Waiting.getLatency();

				nextPatient->reorderTreatments(E_Latency, U_Latency, X_Latency);

				nextPatient->getCurrTreatment()->MoveToWait(this, nextPatient);
			}
			else
				break;
			leavingTime = 0;
		}

		nextPatient = nullptr;
		leavingTime = 0;

		while (latePatients.peek(nextPatient, leavingTime))
		{
			leavingTime = -leavingTime;
			if (leavingTime == currentTimestep)
			{
				latePatients.dequeue(nextPatient, leavingTime);

				int E_Latency = E_Waiting.getLatency();
				int U_Latency = U_Waiting.getLatency();
				int X_Latency = X_Waiting.getLatency();

				nextPatient->reorderTreatments(E_Latency, U_Latency, X_Latency);
				nextPatient->getCurrTreatment()->MoveToWait(this, nextPatient);
			}
			else
				break;

			leavingTime = 0;
		}

		nextPatient = nullptr;
		leavingTime = 0;
		Resource* availableResource = nullptr;


		while (E_Waiting.peek(nextPatient))
		{
			leavingTime = nextPatient->getPriority();
			if (leavingTime <= currentTimestep)
			{
				availableResourcesE.peek(availableResource);
				if (availableResource)
				{
					E_Waiting.dequeue_Latency(nextPatient);
					availableResourcesE.dequeue(availableResource);
					nextPatient->getCurrTreatment()->setResource(availableResource);
					nextPatient->setStatus(SERV);
					nextPatient->getCurrTreatment()->setAssignmentTime(currentTimestep);

					int priority = currentTimestep + nextPatient->getCurrTreatment()->getDuration();
					inTreatmentPatients.enqueue(nextPatient, -priority);
				}
				else
					break;

				int leavingTime = 0;
			}
		}

		nextPatient = nullptr;
		leavingTime = 0;
		availableResource = nullptr;

		while (U_Waiting.peek(nextPatient))
		{
			leavingTime = nextPatient->getPriority();
			if (leavingTime <= currentTimestep)
			{
				availableResourcesU.peek(availableResource);
				if (availableResource)
				{
					U_Waiting.dequeue_Latency(nextPatient);
					availableResourcesU.dequeue(availableResource);
					nextPatient->getCurrTreatment()->setResource(availableResource);
					nextPatient->setStatus(SERV);
					nextPatient->getCurrTreatment()->setAssignmentTime(currentTimestep);

					int priority = currentTimestep + nextPatient->getCurrTreatment()->getDuration();
					inTreatmentPatients.enqueue(nextPatient, -priority);
				}
				else
					break;

				availableResource = nullptr;
				int leavingTime = 0;
			}
		}
		
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

	// Continue Phase 2 Here


}

void Scheduler::add_E_waiting(Patient* waitPatient)
{
	PATIENT_STATUS status = waitPatient->getStatus();
	waitPatient->setStatus(WAIT);
	if (status == EARLY || status == WAIT)
	{
		waitPatient->setPriority(waitPatient->getPT());
		E_Waiting.enqueue_Latency(waitPatient);
	}

	else if (status == LATE)
	{
		int priority = waitPatient->getPT() + (waitPatient->getVT() - waitPatient->getPT()) / 2;
		waitPatient->setPriority(priority);
		E_Waiting.insertSorted(waitPatient);
	}
	else if (status == SERV)
	{
		E_Waiting.insertSorted(waitPatient);
	}
}

void Scheduler::add_U_waiting(Patient* waitPatient)
{
	PATIENT_STATUS status = waitPatient->getStatus();
	waitPatient->setStatus(WAIT);

	if (status == EARLY || status == WAIT)
	{
		waitPatient->setPriority(waitPatient->getPT());
		U_Waiting.enqueue_Latency(waitPatient);
	}

	else if (status == LATE)
	{
		int priority = waitPatient->getPT() + (waitPatient->getVT() - waitPatient->getPT()) / 2;
		waitPatient->setPriority(priority);
		U_Waiting.insertSorted(waitPatient);
	}
	else if (status == SERV)
	{
		U_Waiting.insertSorted(waitPatient);
	}
}

void Scheduler::add_X_waiting(Patient* waitPatient)
{
	PATIENT_STATUS status = waitPatient->getStatus();
	waitPatient->setStatus(WAIT);

	if (status == EARLY || status == WAIT)
	{
		waitPatient->setPriority(waitPatient->getPT());
		X_Waiting.enqueue_Latency(waitPatient);
	}

	else if (status == LATE)
	{
		int priority = waitPatient->getPT() + (waitPatient->getVT() - waitPatient->getPT()) / 2;
		waitPatient->setPriority(priority);
		X_Waiting.insertSorted(waitPatient);
	}
	else if (status == SERV)
	{
		waitPatient->setPriority(waitPatient->getPT());
		X_Waiting.insertSorted(waitPatient);
	}
}




bool Scheduler::loadInputFile()
{
	pUI->getInputFile();
	string filename = "Input/";
	filename += pUI->getFileName();
	// Reads the filename and appends .txt to it if it has length <= 4 or isn't a text file
	if (filename.length() <= 4 || filename.substr(filename.length() - 4) != ".txt")
	{
		filename.append(".txt");
	}
	inFile.open(filename);

	// Checks for any failure when reading the file
	if (inFile.fail() || !inFile.is_open())
	{
		return false;
	}

	// Loads the devices from the file
	loadDevices();

	// Loads the probabilities from the file
	assignProbabilities();

	// Loads the patients from the file
	loadPatients();

	return true;
}

void Scheduler::loadDevices()
{
	// Reads the first line of the file
	int NE, NU, NG;
	inFile >> NE >> NU >> NG;
	for (int i = 0; i < NE; i++)
	{
		// Adds NE Electro Resources to the Available Resources Queue
		Resource* newResource = new ElectroResource();
		availableResourcesE.enqueue(newResource);
	}
	for (int i = 0; i < NU; i++)
	{
		// Adds NU Ultrasound Resources to the Available Resources Queue
		Resource* newResource = new UltrasoundResourse();
		availableResourcesU.enqueue(newResource);
	}
	for (int i = 0; i < NG; i++)
	{
		// Reads the capacity of the Gym Resource from the second line of the file
		int capacity;
		inFile >> capacity;
		// Adds NG Gym Resources to the Available Resources Queue
		Resource* newResource = new GymResource(capacity);
		availableResourcesX.enqueue(newResource);
	}
}

void Scheduler::assignProbabilities()
{
	// Reads the probabilities from the third line of the file
	inFile >> pCancel >> pResc;
}

void Scheduler::loadPatients()
{
	// Reads the number of patients from the fourth line of the file
	inFile >> allPatientsCount;
	for (int i = 0; i < allPatientsCount; i++)
	{
		char type;
		int PT, VT, numT;
		inFile >> type >> PT >> VT >> numT;

		Patient* patient = new Patient(type, PT, VT);
		patient->generateTreatments(numT, inFile);

		allPatients.enqueue(patient);

		patient = nullptr;
	}
}

Scheduler::~Scheduler()
{
	// Deletes initialised UI pointer
	delete pUI;

	// Deletes all patients, They should all be in the finished patients list
	// So just pop each patient and delete them
	Patient* toDelete = nullptr;
	while (finishedPatients.pop(toDelete))
	{
		delete toDelete;
		toDelete = nullptr;
	}

	// Deletes all resources
	Resource* toDeleteR = nullptr;
	while (availableResourcesE.dequeue(toDeleteR))
	{
		delete toDeleteR;
		toDeleteR = nullptr;
	}
	while (availableResourcesU.dequeue(toDeleteR))
	{
		delete toDeleteR;
		toDeleteR = nullptr;
	}
	while (availableResourcesX.dequeue(toDeleteR))
	{
		delete toDeleteR;
		toDeleteR = nullptr;
	}
}

