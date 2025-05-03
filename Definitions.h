#pragma once
#include <string>

enum PATIENT_TYPE
{
	NORMAL,
	RECOVERING
};

enum OPERATION_MODE 
{
	INTERACTIVE,
	SILENT,
	INVALID // In case invalid character is entered, (won't be used unless an error occurs.)
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

enum TREATMENT_TYPE
{
	ELECTROTREATMENT,
	ULTRATREATMENT,
	GYMTREATMENT
};

enum {
	// Instead of using a global variable or the #define directive
	MAX_SIZE = 1000,
	MAX_RESCHEDULE_TIME = 100,
	MAX_RESCHEDULES = 3
};

enum ERR_ID
{
	// Used for Error Messages in UI class
	FILE_NOT_OPEN,
	INVALID_OP_MODE
};

