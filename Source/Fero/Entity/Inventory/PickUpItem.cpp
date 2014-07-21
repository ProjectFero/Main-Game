#include "Fero.h"
#include "PickUpItem.h"
#include "FeroGameMode.h"
APickUpItem::APickUpItem(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{
	Mesh = PCIP.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Mesh"));
	RootComponent = Mesh;
	Component = PCIP.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("Capsule"));
	
}
void APickUpItem::AddToWorld(AFeroGameMode *gm)
{
	gm->AddItem(this);
}
void APickUpItem::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetSimulatePhysics(true);
	Mesh->WakeRigidBody();
}

void APickUpItem::PickedUp()
{
	if (Mesh)
	{
		Mesh->DestroyComponent(); // physical item has been picked up, destroy its visible component
	}
}