#pragma once

//template <typename T>
//class LinkedQueue;

class Treatment;

#include "ElectroTreatment.h"
#include "UltraTreatment.h"
#include "GymTreatment.h"
#include "priQueue.h"

#include "Definitions.h"
#include <fstream>

#include "LinkedQueue.h"



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

	// Flags to know wether or not the patient cancelled and/or rescheduled
	bool fCancelled, fRescheduled;

	PATIENT_TYPE PType;	// Either normal or a recovering patient
	// Normal Patient : this patient must perform the required treatments 
	//					in the same order given in the input file.
	// Recovering Patient : this patient is almost recovered , so he can
	//						perform the required treatments in any order

	PATIENT_STATUS PStatus;

	LinkedQueue<Treatment*> RequiredTreatment; // A list for all required treatments for that patient.
	// For each treatment, you store its duration, assignment time , resource pointer

public:

	Patient(char type, int PT, int VT);


	void setType(char type);


	void setStatus();


	void setStatus(PATIENT_STATUS status);


	PATIENT_STATUS getStatus();

	void generateTreatments(int numT, ifstream& inFile);


	void reorderTreatments(int E_Latency, int U_Latency, int X_Latency);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//											GETTERS & SETTERS											 //
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	int getPID() const;

	int getPT() const;
	void setPT(int newPT);
	
	int getVT() const;
	
	void setPriority(int pri);
	int getPriority();
	
	bool lastTreatment();
	
	void setCurrentDuration(int duration);
	int getCurrentDuration() const;
	
	void setTT(int totalTreatmenttime);
	int getTT();
	
	void setTW();
	int getTW();
	
	void setFT(int finishTime);
	int getFT();

	void cancel();
	bool cancelled();

	void reschedule();
	bool rescheduled();

	PATIENT_TYPE getType();


	// Get the current Treatement to assign the resource to it
	Treatment* getCurrTreatment();
	Treatment* RemoveCurrentTreatment();

	// Use operator overloading for printing
	friend ostream& operator << (ostream& os, const Patient& patient);

	//Copy constructor
	~Patient();



};
