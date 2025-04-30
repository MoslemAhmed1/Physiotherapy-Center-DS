#pragma once

template <typename T>
class LinkedQueue;

class Treatment;

#include "ElectroTreatment.h"
#include "GymTreatment.h"
#include "UltraTreatment.h"
#include "priQueue.h"

#include "Definitions.h"
#include <fstream>



class Patient
{
protected:

	static int numPatients;

	int PID;	// A unique identifier for each patient

	int PT, VT;	// Appointment Time (PT) , Arrival Time (VT)

	int TT, TW; // Total Waiting Time (TW) , Total Treatment Time (TT) ??

	int FT;

	int priority;

	int currentDuration;

	int treatmentFinish;

	PATIENT_TYPE PType;	// Either normal or a recovering patient
	// Normal Patient : this patient must perform the required treatments 
	//					in the same order given in the input file.
	// Recovering Patient : this patient is almost recovered , so he can
	//						perform the required treatments in any order

	PATIENT_STATUS PStatus;

	LinkedQueue<Treatment*> RequiredTreatment; // A list for all required treatments for that patient.
	// For each treatment, you store its duration, assignment time , resource pointer

public:

	Patient(char type, int PT, int VT) : PT(PT), VT(VT)
	{
		PStatus = IDLE;
		setType(type);
		PID = numPatients++;
		priority = PT;
		currentDuration = 0;
		treatmentFinish = 0;
	}

	void setType(char type)
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

	void setStatus()
	{
		if (PT > VT)
			PStatus = EARLY;
		else if (PT < VT)
			PStatus = LATE;
		else
			PStatus = WAIT;
	}

	void setStatus(PATIENT_STATUS status)
	{
		PStatus = status;
	}

	PATIENT_STATUS getStatus() { return PStatus; }

	void generateTreatments(int numT, ifstream& inFile)
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

	void reorderTreatments(int E_Latency, int U_Latency, int X_Latency)
	{
		if (PType == RECOVERING)
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

	int getPID() const { return PID; }
	int getPT() const { return PT; }
	void setPT(int newPT) { PT = newPT; }
	int getVT() const { return VT; }
	void setPriority(int pri) { priority = pri; }
	int getPriority() { return priority; }
	bool lastTreatment() { return (RequiredTreatment.getCount() - treatmentFinish == 1); }
	void setCurrentDuration(int duration) { currentDuration = duration; } //TODO later "Should be equal to the treatment duration"
	int getCurrentDuration() const { return currentDuration; }
	void setTT(int totalTreatmenttime) { TT = totalTreatmenttime; }
	int getTT() { return TT; }
	void setTW() { TW = FT - VT - TT; }
	int getTW() { return TW; }
	void setFT(int finishTime) { FT = finishTime; }
	int getFT() { return FT; }


	// Get the current Treatement to assign the resource to it
	Treatment* getCurrTreatment()
	{
		Treatment* treatment;
		if (RequiredTreatment.peek(treatment)) {
			return treatment;
		}

		return nullptr;
	}

	// Use operator overloading for printing
	friend ostream& operator << (ostream& os, const Patient& patient)
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
	~Patient()
	{
	}

};

int Patient::numPatients = 1;

