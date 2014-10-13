

#include "Fero.h"
#include "Advertisement.h"


AAdvertisement::AAdvertisement(const class FPostConstructInitializeProperties& PCIP)
: AActor(PCIP)
{
	
	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = Mesh;
	Component = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("Capsule"));
}
void AAdvertisement::AddNewPositiveCost(int32 index, int32 Delta)
{
	this->positive_effects.Add(Cost(index, Delta));
}
void AAdvertisement::AddNewNegativeCost(int32 index, int32 Delta)
{
	this->negative_effects.Add(Cost(index, Delta));
}
void AAdvertisement::SetCallableByMultiEntities(bool b)
{
	this->UsableByMoreThanOne = b;
};
void AAdvertisement::AddOwner(int32 index)
{
	this->MyOwner.EntityID = index;
};
