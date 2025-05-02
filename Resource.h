#pragma once
#include "Definitions.h"
#include <iostream>
using namespace std;

class Resource
{
protected:
	RESOURCE_TYPE RType;
	int RID;

public:

	Resource(){}

	Resource(const Resource& other) {}

	int getRID() { return RID; }

	RESOURCE_TYPE getType() { return RType; }

	virtual void PrintInfo(ostream& os) const = 0;

	friend ostream& operator << (ostream& os, const Resource& res)
	{
		res.PrintInfo(os);
		return os;
	}

	virtual ~Resource(){}
};

