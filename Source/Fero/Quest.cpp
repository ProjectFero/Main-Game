

#include "Fero.h"
#include "Quest.h"


UQuest::UQuest(const class FPostConstructInitializeProperties& PCIP)
: Super(PCIP)
{

	FTimerManager().SetTimer(this, &UQuest::Tick, 1.0f, true);
}

void UQuest::SetActive(bool bActiveVal)
{
	if (bActiveVal == true)
	{
		bActive = true;
		QuestBegin();
	}
	else
	{
		bActive = false;
	}
}

void UQuest::Tick()
{
	if (bActive)
	{
		QuestTick();

	}
}


void UQuest::EndQuest()
{

}
