#pragma once
#include "Resource.h"

class UltrasoundResourse : public Resource
{
private:
	static int number;

public:
	UltrasoundResourse() : Resource()
	{
		RID = number;
		RType = ULTRASOUND;
		number++;
	}

	void PrintInfo(ostream& os) const override
	{
		os << RID;
	}
	
	~UltrasoundResourse() { number--; }

	
};

int UltrasoundResourse::number = 1;

