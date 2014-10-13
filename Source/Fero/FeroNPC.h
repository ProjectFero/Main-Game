

#pragma once

#include  "../AdvertisementInterface.h"
#include <memory>
#include "FeroNPC.generated.h"

/**
 * 
 */
UCLASS(config=game)
class FERO_API AFeroNPC : public ACharacter, public AdvertisementInterface
{
	GENERATED_UCLASS_BODY()
protected:
	struct Attribute
	{
	public:
		Attribute(FString n, int32 val, int32 d)
		{
			this->Name = n;
			Value = val;
			Delta = d;
		}
		FString Name;
		int32 Value;
		int32 Delta;
	};
	AdvertisementInterface *Next;
private:
	void ExecuteNegativeEffects(int32 type, int32 Delta);
public:
	bool HasStartedObjective; //Have we started moving?

	float recent_time;
	//Unload on game restart
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FeroNPC)
	bool Death;
	//Use for actions that take a particular amount of time
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = FeroNPC)
	int32 Lock;

	TArray<Attribute> Attributes;

	void ExecuteAdvertisement(AdvertisementInterface *Ad);

	void LockForTurns(AdvertisementInterface *Next);

	bool IsLocked();

	bool IsDead();

	UFUNCTION(BlueprintCallable, Category = "Fero NPC")
		//Used for thread locking
	int32 Lock_Update();

	UFUNCTION(BlueprintCallable, Category = "Fero NPC")
	//Updates the delta values
	void Delta_Tick();


	UFUNCTION(BlueprintCallable, Category = "Fero NPC")
	void ExecuteQueue();

	float ReturnScore(AdvertisementInterface *Ad);

	int ReturnLock() { return Lock; };

	void Navigate(FVector To)
	{
		if (this->Controller != NULL)
		{
			UNavigationSystem::SimpleMoveToLocation(Controller, To);
		}
	};
	FVector ReturnNextPosition()
	{
		return this->Next->GetPosition();
	}
	FVector GetPosition()
	{
		return this->Mesh.Get()->RelativeLocation;
	}
	UCapsuleComponent *ReturnCapsule()
	{
		return this->CapsuleComponent;
	};
	bool CheckIntersectionWithNext()
	{
		return this->CapsuleComponent->IsOverlappingComponent(Next->ReturnCapsule());
	};
};
