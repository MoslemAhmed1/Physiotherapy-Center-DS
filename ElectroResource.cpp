#include "ElectroResource.h"

ElectroResource::ElectroResource() : Resource()
{
	RID = number;
	RType = ELECTRO;
	number++;
}

ElectroResource::ElectroResource(const ElectroResource& other) : Resource(other) {
	RID = number;
	RType = ELECTRO;
	number++;
}

void ElectroResource::PrintInfo(ostream& os) const
{
	os << RID;
}

ElectroResource::~ElectroResource() { number--; }

int ElectroResource::number = 1;