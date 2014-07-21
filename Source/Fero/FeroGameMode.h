// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "FeroGameMode.generated.h"

UCLASS(minimalapi)
class AFeroGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Item")
	void AddItem(class APickUpItem *item); 

	UFUNCTION(BlueprintCallable, Category = "Item")
	void RemoveItem(int32 index);

	TArray<class APickUpItem*> *WorldItems;
};



