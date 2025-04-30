#pragma once
#include "Resource.h"

class ElectroResource : public Resource
{
private:
	static int number;

public:
	ElectroResource()
	{
		RID = number;
		RType = ELECTRO;
		number++;
	}

	void PrintInfo(ostream& os) const override
	{
		os << RID;
	}

	~ElectroResource() { number--; }

	
};

int ElectroResource::number = 1;