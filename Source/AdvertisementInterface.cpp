#include "AdvertisementInterface.h"

int32 AdvertisementInterface::ComputeCompletionTime(int32 WeighedValue)
{
	if (WeighedValue == 0)
		return -1;
	return (100 / WeighedValue) + TimeConstant; //We can include other variables here as well obviously
};
void AdvertisementInterface::SetEntityOwner(int32 ID)
{
	this->MyOwner.EntityID = ID;
	UsableByMoreThanOne = false;
};
int32 AdvertisementInterface::GetOwner() //Use this with "World"
{
	return MyOwner.EntityID;
}
void AdvertisementInterface::BeginUse()
{
	MyOwner.InUse = true;
}
void AdvertisementInterface::EndUse()
{
	MyOwner.InUse = false;
}
bool AdvertisementInterface::IsInUse()
{
	return this->MyOwner.InUse;
};