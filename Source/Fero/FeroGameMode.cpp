// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Fero.h"
#include "Advertisement.h"
#include "FeroNPC.h"
#include <vector>
#include <algorithm> 
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
void AFeroGameMode::AddAdvertisement(AAdvertisement *Advertisement_To_Add)
{
	this->Ads.push_back(Advertisement_To_Add);
};
void AFeroGameMode::AddEntity(AFeroNPC *Entity_To_Add)
{
	this->Entities.push_back(Entity_To_Add);
};
int AFeroGameMode::UpdateEntity(int EntityIndex, int rad)
{
	if (Entities.at(EntityIndex)->IsLocked()) //If we're currently doing another task, don't bother adding another
	{
		if (Entities.at(EntityIndex)->CheckIntersectionWithNext()) //Are we there yet? :/
		{
			Entities.at(EntityIndex)->Lock_Update();

			if (Entities.at(EntityIndex)->ReturnLock() <= 1) //Our lock has ended. Start on the next task
			{
				Entities.at(EntityIndex)->ExecuteQueue();
			}
			
		}
		else if (!Entities.at(EntityIndex)->HasStartedObjective) //We haven't started moving to the objective
		{
			Entities.at(EntityIndex)->Navigate(Entities.at(EntityIndex)->ReturnNextPosition());
		}
		return 0;
	}
	else
	{
		AdList A = FindLocalAds(EntityIndex, rad);
		std::vector<float> Scores = std::vector<float>();
		for (int i = 0; i < (int)A.size(); i++)
		{
			Scores.push_back(Entities.at(EntityIndex)->ReturnScore(A.at(i)));
#ifdef AI_DEBUG_MODE_GOALS
			string temp = string(A.at(i).get()->Name.c_str());
			temp.append(" Advertisment: ");
			temp.append(std::to_string(Scores.at(i)));
			std::cout << temp << "\n";
#endif
		}
#ifdef AI_DEBUG_MODE_GOALS
		Entities.at(EntityIndex).get()->PrintGoals();
#endif
		int index;
		index = std::distance(Scores.begin(), std::max_element(Scores.begin(), Scores.end())); //Where does the highest value occur?
		while ((A.at(index)->IsInUse())
			|| ((A.at(index)->IsInUse() && A.at(index)->UsableByMoreThanOne))) //Either its not in use, or its in use, but sharable between people
		{
			A.erase(A.begin() + index); //So we erase it from the vector, and get the next highest
			Scores.erase(Scores.begin() + index);
			index = std::distance(Scores.begin(), std::max_element(Scores.begin(), Scores.end())); //Where does the highest value occur?
		}
		if (!A.at(index)->UsableByMoreThanOne)
			A.at(index)->SetEntityOwner(EntityIndex); //If it isn't sharable, set the owner to ourself. We could eventually set this up as a job application idealogy 
#ifdef AI_DEBUG_MODE_GOALS
		Entities.at(EntityIndex).get()->LockForTurns(A.at(index), index);
#else
		Entities.at(EntityIndex)->LockForTurns(A.at(index));
#endif
		//Entities.at(EntityIndex).get()->ExecuteAdvertisment(A.at(index)); //Execute the goal as usual. Eventually we'll have a queu
		A.at(index)->BeginUse();

		return 1;
	}
};
AdList AFeroGameMode::FindLocalAds(int EntityIndex, int rad) //Looks to see how many advertisements are within walking distance of the entity
{
	AdList TempList = AdList();
	FVector temp = Entities.at(EntityIndex)->GetPosition();
	for (int i = 0; i < (int)Ads.size(); i++)
	{
		if (rad > FVector::Dist(temp,Ads.at(i)->GetPosition())) //Simple bounding sphere
			TempList.push_back(Ads.at(i));
	};
	return TempList;
};