#include "UltrasoundResourse.h"

UltrasoundResourse::UltrasoundResourse() : Resource()
{
	RID = number;
	RType = ULTRASOUND;
	number++;
}

void UltrasoundResourse::PrintInfo(ostream& os) const
{
	os << RID;
}

UltrasoundResourse::~UltrasoundResourse() { number--; }
int UltrasoundResourse::number = 1;
