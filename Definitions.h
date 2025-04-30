#pragma once
#include <string>

enum PATIENT_TYPE
{
	NORMAL,
	RECOVERING
};

enum PATIENT_STATUS
{
	IDLE,
	EARLY,
	LATE,
	WAIT,
	SERV,
	FNSH
};

enum RESOURCE_TYPE
{
	ULTRASOUND,
	ELECTRO,
	GYM
};

enum
{
	// Instead of using a global variable or the #define directive
	MAX_SIZE = 1000
};

enum errID
{
	// Used for Error Messages in UI class
	FILE_NOT_OPEN
};

