#include "Patient.h"

Patient::Patient(char type, int PT, int VT) : PT(PT), VT(VT)
{
	PStatus = IDLE;
	setType(type);
	PID = numPatients;
	priority = PT;
	currentDuration = 0;
	treatmentFinish = 0;
	numPatients++;
	fCancelled = false;
	fRescheduled = false;
}

void Patient::setType(char type)
{
	switch (type)
	{
	case 'N':
		PType = NORMAL;
		break;
	case 'R':
		PType = RECOVERING;
		break;
	default:
		break;
	}
}

void Patient::setStatus()
{
	if (PT > VT)
		PStatus = EARLY;
	else if (PT < VT)
		PStatus = LATE;
	else
		PStatus = WAIT;
}

void Patient::setStatus(PATIENT_STATUS status)
{
	PStatus = status;
}

PATIENT_STATUS Patient::getStatus() { return PStatus; }

void Patient::generateTreatments(int numT, ifstream& inFile)
{
	Treatment* treatment;

	for (int i = 0; i < numT; i++)
	{
		char type;
		int duration;
		inFile >> type >> duration;

		type = toupper(type);

		switch (type)
		{
		case 'E':
			treatment = new ElectroTreatment(duration);
			RequiredTreatment.enqueue(treatment);
			treatment = nullptr;
			break;
		case 'U':
			treatment = new UltraTreatment(duration);
			RequiredTreatment.enqueue(treatment);
			treatment = nullptr;
			break;
		case 'X':
			treatment = new GymTreatment(duration);
			RequiredTreatment.enqueue(treatment);
			treatment = nullptr;
			break;
		}

	}
}

void Patient::reorderTreatments(int E_Latency, int U_Latency, int X_Latency)
{
	if (PType == RECOVERING && RequiredTreatment.getCount() > 1)
	{
		priQueue<Treatment*> temp;
		Treatment* treatment = nullptr;
		while (RequiredTreatment.dequeue(treatment))
		{
			if (treatment->getType() == ELECTROTREATMENT)
			{
				temp.enqueue(treatment, -E_Latency);
			}
			else if (treatment->getType() == ULTRATREATMENT)
			{
				temp.enqueue(treatment, -U_Latency);
			}
			else
			{
				temp.enqueue(treatment, -X_Latency);
			}
		}

		treatment = nullptr;
		int dummy;

		while (temp.dequeue(treatment, dummy))
		{
			RequiredTreatment.enqueue(treatment);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//											GETTERS & SETTERS											 //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

int Patient::getPID() const { return PID; }
int Patient::getPT() const { return PT; }
void Patient::setPT(int newPT) { PT = newPT; }
int Patient::getVT() const { return VT; }
void Patient::setPriority(int pri) { priority = pri; }
int Patient::getPriority() { return priority; }
bool Patient::lastTreatment() { return (RequiredTreatment.getCount() - treatmentFinish == 1); }
void Patient::setCurrentDuration(int duration) { currentDuration = duration; } //TODO later "Should be equal to the treatment duration"
int Patient::getCurrentDuration() const { return currentDuration; }
void Patient::setTT(int totalTreatmenttime) { TT = totalTreatmenttime; }
int Patient::getTT() { return TT; }
void Patient::setTW() { TW = FT - VT - TT; }
int Patient::getTW() { return TW; }
void Patient::setFT(int finishTime) { FT = finishTime; }
int Patient::getFT() { return FT; }


void Patient::cancel() {
	fCancelled = true;
}

bool Patient::cancelled() {
	return fCancelled;
}

void Patient::reschedule() {
	fRescheduled = true;
}

bool Patient::rescheduled() {
	return fRescheduled;
}

PATIENT_TYPE Patient::getType() {
	return PType;
}



// Get the current Treatement to assign the resource to it
Treatment* Patient::getCurrTreatment()
{
	Treatment* treatment;
	if (RequiredTreatment.peek(treatment)) {
		return treatment;
	}

	return nullptr;
}

Treatment* Patient::RemoveCurrentTreatment()
{
	Treatment* treatment = nullptr;
	if (RequiredTreatment.dequeue(treatment))
	{
		return treatment;
	}
	return nullptr;
}

// Use operator overloading for printing
ostream& operator << (ostream& os, const Patient& patient)
{
	Treatment* currTreatment;
	bool isTreatment = patient.RequiredTreatment.peek(currTreatment);
	switch (patient.PStatus)
	{
	case IDLE:
		os << 'P' << patient.PID << '_' << patient.VT;
		break;
	case SERV:
		os << 'P' << patient.PID << '_';
		if (isTreatment)
			os << *currTreatment;
		break;
	default:
		os << patient.PID;
		break;
	}
	return os;
}

//Copy constructor
Patient::~Patient()
{
}

int Patient::numPatients = 1;
