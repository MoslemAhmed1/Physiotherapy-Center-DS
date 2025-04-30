#pragma once
#include <string>

#include "LinkedQueue.h"
#include "EUWaitingList.h"
#include "XWaitingList.h"

#include "ArrayStack.h"

#include "priQueue.h"
#include "EarlyPList.h"

#include "ElectroResource.h"
#include "UltrasoundResourse.h"
#include "GymResource.h"

#include "Treatment.h"
#include "ElectroTreatment.h"
#include "UltraTreatment.h"
#include "GymTreatment.h"

#include "Patient.h"
#include "Definitions.h"

#include <iostream>
#include <conio.h>

using namespace std;

class UI
{
private:
	string fileName;

public:
	// Constructor
	UI() {
		fileName = "";
	}
	// Prints All the Lists and their contents
	void printMaster(
		int timeStep,
		LinkedQueue<Resource*>* rE, LinkedQueue<Resource*>* rU, LinkedQueue<Resource*>* rX,
		EUWaitingList* uW, EUWaitingList* eW,
		XWaitingList* xW,
		LinkedQueue<Patient*>* all, EarlyPList* eP, ArrayStack<Patient*>* fnsh,
		priQueue<Patient*>* late, priQueue<Patient*>* inTrt
	)
	{
		cout << "Current Timestep : " << timeStep << endl;

		cout << "=============== ALL List ===============" << endl;
		cout << all->getCount() << " patients remaining: ";
		all->PrintInfo();
		cout << "=============== Waiting Lists ===============" << endl;
		cout << eW->getCount() << " E-therapy patients: ";
		eW->PrintInfo();
		cout << uW->getCount() << " U-therapy patients: ";
		uW->PrintInfo();
		cout << xW->getCount() << " X-therapy patients: ";
		xW->PrintInfo();

		cout << "=============== Early List ===============" << endl;
		cout << eP->getCount() << " patients: ";
		eP->PrintInfo();

		cout << "=============== Late List ===============" << endl;
		cout << late->getCount() << " patients: ";
		late->PrintInfo();

		cout << "=============== Avail E-devices ===============" << endl;
		cout << rE->getCount() << " Electro device: ";
		rE->PrintInfo();

		cout << "=============== Avail U-devices ===============" << endl;
		cout << rU->getCount() << " Ultra device: ";
		rU->PrintInfo();

		cout << "=============== Avail X-rooms ===============" << endl;
		cout << rX->getCount() << " rooms: ";
		rX->PrintInfo();

		cout << "=============== In-treatment List ===============" << endl;
		cout << inTrt->getCount() << " ==> ";
		inTrt->PrintInfo();

		cout << "=============== Finished List ===============" << endl;
		cout << fnsh->getCount() << " Finished Patients:  ";
		fnsh->PrintInfo();

		cout << "-----------------------------------------------\n";
	}
	// Prints a nice welcome message :)
	void printWelcomeMessage() {
		cout << "+-----------------------------+\n";
		cout << "|                             |\n";
		cout << "|      __  __ ____ ____       |\n";
		cout << "|     |  \\/  / ___/ ___|      |\n";
		cout << "|     | |\\/| \\___ \\___ \\      |\n";
		cout << "|     | |  | |___) |__) |     |\n";
		cout << "|     |_|  |_|____/____/      |\n";
		cout << "|                             |\n";
		cout << "+-----------------------------+\n";
		cout << "Welcome to the medicinal scheduling system (MSS).\n";
		cout << "\n==========================================\n";
	}
	
	// Gets the file name and stores it in the fileName data member
	void getInputFile() {
		cout << "\nEnter the filename: ";
		cin >> fileName;
	}
	
	// Gets Mode of operation (Silent or Interactive)
	OPERATION_MODE getOpMode() {
		char opMode;
		cout << "\nEnter mode of operation (S->Silent, I->Interactive): ";
		cin >> opMode;
		opMode = toupper(opMode);
		switch (opMode) {
		case 'S':
			return SILENT;
		case 'I':
			return INTERACTIVE;
		default:
			return INVALID;
		}
	}
	
	// Checks if the user wants to proceed with the simulation or exit
	bool proceed()
	{
		cout << "\n\n===========================================\n\n";
		cout << "Press any key to continue. Press ESC to exit." << endl;


		// Wait for a key press
		int key = _getch();
		// Check which key was pressed
		if (key == 27) { // ASCII value for ESC
			cout << "Exiting simulation..." << endl;
			return false;
		}
		else {
			return true;
		}
	}
	
	// Prints different error messages based on the error ID
	void printErrorMsg(ERR_ID id)
	{
		switch (id) // in case more error types are added
		{
		case(FILE_NOT_OPEN):
			cout << "\n==ERROR! FILE NOT FOUND OR COULDN'T BE OPENED!==\n";
			break;
		case(INVALID_OP_MODE):
			cout << "\n==ERROR! INVALID OPERATION MODE ENTERED!==\n";
		default:
			break;
		}
	}

	// Prints Exit Message 
	void printEndMessage(OPERATION_MODE opMode) {
		if (opMode == SILENT) {
			cout << "\nSilent Mode, Simulation ends, Output file created";
		}
		else if (opMode == INTERACTIVE) {
			cout << "\nExiting simulation...";
		}
	}

	// returns the file name
	string getFileName() { return fileName; }
	
	// Sets File Name
	void setFileName(string name) { fileName = name; }

};
