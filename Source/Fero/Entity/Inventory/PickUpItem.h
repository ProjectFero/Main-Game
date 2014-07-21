#pragma once

//#include "GameFramework/Actor.h"
#include "PickUpItem.generated.h"

UCLASS()
class APickUpItem : public AActor
{
	GENERATED_UCLASS_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Item")
	void AddToWorld(class AFeroGameMode *gm);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 Value;

	UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "Item: Used"))
	virtual void Used();

	UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "Item: Dropped"))
	virtual void Dropped();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	TSubobjectPtr<class UCapsuleComponent> Component;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	TSubobjectPtr<class UStaticMeshComponent> Mesh;

	virtual void BeginPlay() override;
	void PickedUp();

	bool IsIntersect(UCapsuleComponent *point)
	{
		Component.Get()->SetWorldLocation(Mesh.Get()->RelativeLocation);
		return Component->IsOverlappingComponent(point);
	}
private:
	float Radius;
};