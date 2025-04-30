#pragma once
#include "Resource.h"

class UltrasoundResourse : public Resource
{
private:
	static int number;

public:
	UltrasoundResourse();

	void PrintInfo(ostream& os) const;
	
	
	~UltrasoundResourse();

};


