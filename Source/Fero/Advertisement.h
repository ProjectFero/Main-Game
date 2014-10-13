

#pragma once

#include "GameFramework/Actor.h"
#include <vector>
#include  "../AdvertisementInterface.h"
#include "Advertisement.generated.h"

/**
 * 
 */
UCLASS()
class FERO_API AAdvertisement : public AActor, public AdvertisementInterface
{
	GENERATED_UCLASS_BODY()


public:

	UFUNCTION(BlueprintCallable, Category = "Advertisement")
	void AddNewPositiveCost(int32 index, int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Advertisement")
	void AddNewNegativeCost(int32 index, int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Advertisement")
	void SetCallableByMultiEntities(bool b);

	UFUNCTION(BlueprintCallable, Category = "Advertisement")
	void AddOwner(int32 index);

	UCapsuleComponent *ReturnCapsule()
	{
		return this->Component;
	};
	FVector GetPosition()
	{
		return this->Mesh.Get()->RelativeLocation;
	};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Advertisement)
	TSubobjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Advertisement)
	TSubobjectPtr<class UCapsuleComponent> Component;
};
