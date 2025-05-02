#pragma once
#include "Resource.h"

class ElectroResource : public Resource
{
private:
	static int number;

public:
	ElectroResource();
	ElectroResource(const ElectroResource& other);

	void PrintInfo(ostream& os) const;

	~ElectroResource();
};

