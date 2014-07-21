// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Fero.h"
#include "FeroGameMode.h"

AFeroGameMode::AFeroGameMode(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	WorldItems = new TArray<class APickUpItem*>();
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FObjectFinder<UClass> PlayerPawnBPClass(TEXT("Class'/Game/Blueprints/MyCharacter.MyCharacter_C'"));
	if (PlayerPawnBPClass.Object != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Object;

	}
}
void AFeroGameMode::AddItem(class APickUpItem *item)
{
	this->WorldItems->Add(item);
}
void AFeroGameMode::RemoveItem(int32 index)
{
	this->WorldItems->RemoveAt(index);
}