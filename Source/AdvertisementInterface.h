#pragma once
#include <vector>
#include <math.h>
#include "Array.h"
#include "HelperFunctions.h"

class FERO_API AdvertisementInterface
{

public:
	AdvertisementInterface()
	{
		TimeConstant = 0;
		negative_effects = TArray<Cost>();
		positive_effects = TArray<Cost>();
		Name = "Default";
		UsableByMoreThanOne = true;
		MyOwner.InUse = false;
	}

	Ownership MyOwner;

	int32 ComputeCompletionTime(int32 WeighedValue);

	void SetEntityOwner(int32 ID);

	//Use this with "World"
	int32 GetOwner();
	void BeginUse();
	void EndUse();
	bool IsInUse();

	bool UsableByMoreThanOne;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Advertisement)
	int32 TimeConstant;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Advertisement)
	FString Name;

	virtual FVector GetPosition() = 0;
	virtual UCapsuleComponent *ReturnCapsule() = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Advertisement)
	TArray<Cost> negative_effects; //How badly does this effect us?
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Advertisement)
	TArray<Cost> positive_effects; //What good does this do for us?


};