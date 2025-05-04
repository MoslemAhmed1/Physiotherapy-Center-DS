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
	totalPenaltyTime = 0;
	earlyNum = 0;
	lateNum = 0;
}

void Scheduler::Simulate()
{
	// Load Input File
	pUI->printWelcomeMessage();
	bool opened = loadInputFile();
	while (!opened) {
		pUI->printErrorMsg(FILE_NOT_OPEN);
		opened = loadInputFile();
	}

	opMode = pUI->getOpMode();
	while (opMode == INVALID) {
		pUI->printErrorMsg(INVALID_OP_MODE);
		opMode = pUI->getOpMode();
	}

	while (finishedPatients.getCount() != allPatientsCount)
	{	
		// 1- All Patients to (Early/Late/Wait)
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
					earlyNum++;
					earlyPatients.enqueue(nextPatient, -PT);
				}

				else if (status == LATE)
				{
					lateNum++;
					int penalty = (VT - PT) / 2;
					totalPenaltyTime += penalty;
					latePatients.enqueue(nextPatient, -(VT + penalty));
				}

				else if (status == WAIT)
				{
					int E_Latency = E_Waiting.getLatency();
					int U_Latency = U_Waiting.getLatency();
					int X_Latency = X_Waiting.getLatency();

					nextPatient->reorderTreatments(E_Latency, U_Latency, X_Latency);

					// Could add a function "AssignToWait" in the patient class to perform this ?
					nextPatient->getCurrTreatment()->MoveToWait(this, nextPatient);	
				}
			}
			else break;
		}

		nextPatient = nullptr;
		int leavingTime = 0;

		// 2.1- Early Patients to Wait
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
		}

		nextPatient = nullptr;
		leavingTime = 0;

		// 2.2- Late Patients to Wait
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
		}

		// 3- InTreatment to Finish / Wait
		while (inTreatmentPatients.peek(nextPatient, leavingTime))
		{
			leavingTime = -leavingTime;
			if (leavingTime == currentTimestep)
			{
				inTreatmentPatients.dequeue(nextPatient, leavingTime);

				// Return the patients resource to the available resources list
				Resource* resourcePtr = nextPatient->getCurrTreatment()->getResource();
				if (resourcePtr)
				{
					if (resourcePtr->getType() == ELECTRO)
					{
						availableResourcesE.enqueue(resourcePtr);
					}
					else if (resourcePtr->getType() == ULTRASOUND)
					{
						availableResourcesU.enqueue(resourcePtr);
					}
					else if (resourcePtr->getType() == GYM)
					{
						GymResource* gymResource = dynamic_cast<GymResource*>(resourcePtr);
						if (gymResource && (gymResource->getCurrPatients() == gymResource->getCapacity()))
						{
							availableResourcesX.enqueue(resourcePtr);
						}
					}
				}

				// Handle deleting the required treatment
				nextPatient->RemoveCurrentTreatment();

				// Patient has finished all his treatments
				if (!nextPatient->getCurrTreatment())
				{
					nextPatient->setStatus(FNSH);
					nextPatient->setFT(currentTimestep);
					nextPatient->setTW();
					finishedPatients.push(nextPatient);
				}
				else // Move patient to the next waiting list
				{
					int E_Latency = E_Waiting.getLatency();
					int U_Latency = U_Waiting.getLatency();
					int X_Latency = X_Waiting.getLatency();

					nextPatient->reorderTreatments(E_Latency, U_Latency, X_Latency);
					nextPatient->getCurrTreatment()->MoveToWait(this, nextPatient);
				}
			}
			else
				break;
		}

		nextPatient = nullptr;
		leavingTime = 0;
		Resource* availableResource = nullptr;

		// 4.1- E-Waiting to Intreatment
		while (E_Waiting.peek(nextPatient))
		{
			leavingTime = nextPatient->getPriority();
			if (leavingTime <= currentTimestep)
			{
				availableResourcesE.peek(availableResource);
				if (nextPatient->getCurrTreatment()->CanAssign(availableResource))
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

				availableResource = nullptr;
			}
		}

		nextPatient = nullptr;
		leavingTime = 0;
		availableResource = nullptr;

		// 4.2- U-Waiting to Intreatment
		while (U_Waiting.peek(nextPatient))
		{
			leavingTime = nextPatient->getPriority();
			if (leavingTime <= currentTimestep)
			{
				availableResourcesU.peek(availableResource);
				if (nextPatient->getCurrTreatment()->CanAssign(availableResource))
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
			}
		}

		nextPatient = nullptr;
		leavingTime = 0;
		availableResource = nullptr;

		// 4.3- X-Waiting to Intreatment
		while (X_Waiting.peek(nextPatient))
		{
			leavingTime = nextPatient->getPriority();
			if (leavingTime <= currentTimestep)
			{
				availableResourcesX.peek(availableResource);
				if (nextPatient->getCurrTreatment()->CanAssign(availableResource))
				{
					X_Waiting.dequeue_Latency(nextPatient);
					nextPatient->getCurrTreatment()->setResource(availableResource);

					// Check if resource is full (Capacity == currPatients)
					if (!nextPatient->getCurrTreatment()->CanAssign(availableResource))
					{
						availableResourcesX.dequeue(availableResource);
					}

					nextPatient->setStatus(SERV);
					nextPatient->getCurrTreatment()->setAssignmentTime(currentTimestep);

					int priority = currentTimestep + nextPatient->getCurrTreatment()->getDuration();
					inTreatmentPatients.enqueue(nextPatient, -priority);
				}
				else
					break;

				availableResource = nullptr;
			}
		}

		nextPatient = nullptr;
		leavingTime = 0;
		availableResource = nullptr;

		// 5.1- Cancellation
		int randomNum = Facilities::generateRandomNumber(0, 100);
		if (randomNum < pCancel && !X_Waiting.isEmpty())
		{
			Patient* cancelledPatient = nullptr;
			if (X_Waiting.cancel(cancelledPatient)) 
			{
				// Set Status , FT , TT , TW ... 
				Treatment* cancelledTreatment = cancelledPatient->RemoveCurrentTreatment();
				cancelledPatient->setStatus(FNSH);
				cancelledPatient->setFT(currentTimestep);
				cancelledPatient->setTT(cancelledPatient->getTT() - cancelledTreatment->getDuration());
				cancelledPatient->setTW();
				finishedPatients.push(cancelledPatient);	
			}
		}
		
		// 5.2- Rescheduling
		randomNum = Facilities::generateRandomNumber(0, 100);
		if (randomNum < pResc && !earlyPatients.isEmpty())
		{
			earlyPatients.reschedule();
		}
		
		if (opMode == INTERACTIVE) {	
			pUI->printMaster(
				currentTimestep, &availableResourcesE, &availableResourcesU, &availableResourcesX,
				&U_Waiting, &E_Waiting, &X_Waiting, &allPatients, &earlyPatients, &finishedPatients,
				&latePatients, &inTreatmentPatients
			);

			bool proceed = pUI->proceed();
			if (!proceed)
				opMode = SILENT; // Change operation mode to silent
								 // This still produces the output file.
								 // Under the assumption that the user pressed esc to skip the interactive mode
		}

		currentTimestep++;
	}
	pUI->printEndMessage(opMode);
	generateOutputFile();
}

void Scheduler::AddToWait_E(Patient* waitPatient)
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

void Scheduler::AddToWait_U(Patient* waitPatient)
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

void Scheduler::AddToWait_X(Patient* waitPatient)
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

bool Scheduler::generateOutputFile() {
	string filename = "Output/";
	filename += pUI->getFileName();
	// Reads the filename and appends .txt to it if it has length <= 4 or isn't a text file
	if (filename.length() <= 4 || filename.substr(filename.length() - 4) != ".txt") {
		filename.append(".txt");
	}
	outFile.open(filename);

	if (outFile.fail() || !outFile.is_open()) {
		return false;
	}

	Patient* toPrint = nullptr;
	int nNum = 0, rNum = 0;  
	int cancelledNum = 0, rescheduledNum = 0;
	int allTT = 0, nTT = 0, rTT = 0;
	int allWT = 0, nWT = 0, rWT = 0;

	outFile << "PID" << "\tPType" << "\tPT" << "\tVT" << "\tFT" << "\tWT" << "\tTT" << "\tCancel" << "\tResc" << '\n';

	while (finishedPatients.pop(toPrint)) {
		// PID
		outFile << 'P' << toPrint->getPID() << '\t';
		
		// pType
		if (toPrint->getType() == NORMAL) {
			nNum++;
			outFile << 'N';
			nTT += toPrint->getTT();
			nWT += toPrint->getTW();
		}
		else {
			rNum++;
			outFile << 'R';
			rTT += toPrint->getTT();
			rWT += toPrint->getTW();
		}

		allTT += toPrint->getTT();
		allWT += toPrint->getTW();

		outFile << '\t' << toPrint->getPT(); // PT
		outFile << '\t' << toPrint->getVT(); // VT
		outFile << '\t' << toPrint->getFT(); // FT
		outFile << '\t' << toPrint->getTW(); // WT
		outFile << '\t' << toPrint->getTT(); // TT

		// Cancel
		outFile << '\t';
		if (toPrint->cancelled()) {
			cancelledNum++;
			outFile << 'T';
		}
		else {
			outFile << 'F';
		}

		// Resc
		outFile << '\t';
		if (toPrint->getNumReschedules() != 0) {
			rescheduledNum++;
			outFile << 'T';
		}
		else {
			outFile << 'F';
		}

		outFile << '\n';
	}

	outFile << "\nTotal number of timesteps = " << currentTimestep;
	outFile << "\nTotal number of all, N, and R patients = " << allPatientsCount << ", " << nNum << ", " << rNum;
	outFile << "\nAverage total waiting time for all, N, and R patients = "
		<< (allPatientsCount == 0 ? 0.0 : static_cast<double>(allWT) / allPatientsCount) << ", "
		<< (nNum == 0 ? 0.0 : static_cast<double>(nWT) / nNum) << ", "
		<< (rNum == 0 ? 0.0 : static_cast<double>(rWT) / rNum);

	outFile << "\nAverage total treatment time for all, N, and R patients = "
		<< (allPatientsCount == 0 ? 0.0 : static_cast<double>(allTT) / allPatientsCount) << ", "
		<< (nNum == 0 ? 0.0 : static_cast<double>(nTT) / nNum) << ", "
		<< (rNum == 0 ? 0.0 : static_cast<double>(rTT) / rNum);

	outFile << "\nPercentage of patients of an accepted cancellation (%) = "
		<< (allPatientsCount == 0 ? 0.0 : static_cast<double>(cancelledNum) / allPatientsCount * 100) << '%';

	outFile << "\nPercentage of patients of an accepted rescheduling (%) = "
		<< (allPatientsCount == 0 ? 0.0 : static_cast<double>(rescheduledNum) / allPatientsCount * 100) << '%';

	outFile << "\nPercentage of early patients (%) = "
		<< (allPatientsCount == 0 ? 0.0f : static_cast<float>(earlyNum) / allPatientsCount * 100) << '%';

	outFile << "\nPercentage of late patients (%) = "
		<< (allPatientsCount == 0 ? 0.0f : static_cast<float>(lateNum) / allPatientsCount * 100) << '%';

	outFile << "\nAverage late penalty = "
		<< (lateNum == 0 ? 0.0f : static_cast<float>(totalPenaltyTime) / lateNum);

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

