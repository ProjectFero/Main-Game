

#pragma once

#include "GameFramework/Actor.h"
#include "PickUpItem.h"
#include "Potion.generated.h"


/**
 * 
 */
UCLASS()
class FERO_API APotion : public APickUpItem
{
	GENERATED_UCLASS_BODY()
public:
	UENUM()
	enum PotionTypes
	{
		Health= 0,
		Mana,
		Defense,
		Attack,
		Support,
	};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	int32 Type;

	virtual void Used();
};
