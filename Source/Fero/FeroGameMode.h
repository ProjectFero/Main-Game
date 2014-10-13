// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include <memory>
#include <vector>
#include "FeroGameMode.generated.h"


typedef std::vector<class AAdvertisement*> AdList;
typedef std::vector<class AFeroNPC*> EntityList;

UCLASS(minimalapi)
class AFeroGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()
private:
	AdList Ads;
	EntityList Entities;
public:
	UFUNCTION(BlueprintCallable, Category = "Item")
	void AddItem(class APickUpItem *item); 

	UFUNCTION(BlueprintCallable, Category = "Item")
	void RemoveItem(int32 index);

	TArray<class APickUpItem*> *WorldItems;

	UFUNCTION(BlueprintCallable, Category = "NPC")
	void AddAdvertisement(class AAdvertisement *Advertisement_To_Add);
	UFUNCTION(BlueprintCallable, Category = "NPC")
	void AddEntity(class AFeroNPC *Entity_ToAdd);
	UFUNCTION(BlueprintCallable, Category = "NPC")
	int32 UpdateEntity(int32 EntityIndex, int32 rad);

	AdList FindLocalAds(int EntityIndex, int rad);
};



