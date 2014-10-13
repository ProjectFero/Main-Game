

#include "Fero.h"
#include "../AdvertisementInterface.cpp"
#include "FeroNPC.h"


AFeroNPC::AFeroNPC(const class FPostConstructInitializeProperties& PCIP)
: ACharacter(PCIP), AdvertisementInterface()
{
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	


	this->Attributes = TArray<Attribute>();
	Attributes.Add(Attribute("Health", 100, 1));
	Attributes.Add(Attribute("Wealth", 100, 1));
	Attributes.Add(Attribute("Food", 5, 1));
	Death = false;
	//this->SelfAdvertisement.get()->Name = "Socialization";
	//this->SelfAdvertisement.get()->UsableByMoreThanOne = true;
}
bool AFeroNPC::IsLocked()
{
	return (Lock > 1);
}
bool AFeroNPC::IsDead()
{
	if ((Attributes[0].Value <= 0) || (Attributes[1].Value <= 0))
		Death = true;
	else
		Death = false;
	return Death;
};
int32 AFeroNPC::Lock_Update() //Returns 0 when the entity is still locked, returns 1 when lock ends.
{
	int i = 1;
	if (Lock > 1)
	{
		Lock -= 1;
		i = 0;
	}
	//Other update code goes here

	return i;
};
void AFeroNPC::LockForTurns(AdvertisementInterface *Next)
{
	Lock = Next->ComputeCompletionTime(this->Attributes[GoalVarType::Hunger].Value);
	this->Next = Next;
};
void AFeroNPC::Delta_Tick()
{
	if (Attributes[0].Value  > 100)
		Attributes[0].Value = 100;
	if (Attributes[2].Value  > 100)
		Attributes[2].Value = 100;
	for (auto i = Attributes.CreateIterator(); i; ++i)
	{
		i->Value -= i->Delta;
	};
};
void AFeroNPC::ExecuteNegativeEffects(int32 type, int32 Delta)
{
	Attributes[type].Value -= Delta;
}
void AFeroNPC::ExecuteAdvertisement(AdvertisementInterface *Ad)
{

	int itt;
	for (itt = 0; itt < Ad->ComputeCompletionTime(Attributes[0].Value); itt++) //Update for how many times it takes to complete this task
		Delta_Tick();
	for (auto i = Ad->positive_effects.CreateIterator(); i; ++i)
	{
		Attributes[i->type].Value += i->DiminishingReturn;
	}
	if (Attributes[0].Value > 100)
		Attributes[0].Value = 100;
	if (Attributes[2].Value > 100)
		Attributes[2].Value = 100;
	for (auto i = Ad->negative_effects.CreateIterator(); i; ++i)
	{
		ExecuteNegativeEffects(i->type, i->DiminishingReturn);
	}
	Ad->EndUse();
};
void AFeroNPC::ExecuteQueue()
{
	ExecuteAdvertisement(Next);
};
float AFeroNPC::ReturnScore(AdvertisementInterface *Ad)
{
	TArray<float> arr = TArray<float>();
	for (auto j = Attributes.CreateConstIterator(); j; j++) //We could eventually precompute timeaspect
	{
		arr[j.GetIndex()] = Monitization::TimeAspect(Ad->ComputeCompletionTime(this->Attributes[0].Delta), j->Delta, j->Value);
	}
	float t = 0;
	int avgscore1 = 0;
	for(auto i = Ad->positive_effects.CreateConstIterator(); i; ++i)
	{

		int score1, score2;
		score1 = score2 = 0;
		score2 = i->DiminishingReturn;
		score1 = Attributes[i->type].Value;
		score2 += score1; //Score2 = Future score
		avgscore1 += score1;
		if (i->type == GoalVarType::Health || i->type == GoalVarType::Hunger)
		{
			if (score2 > 100)
				score2 = 100;
			if (score1 > 100)
				score1 = 100;
		}
		float m = 0;
		for (int j = 0; j < arr.Num(); j++)
		{
			if (j != i->type) //We should account for all variables besides the one we're trying to improve
			{
				m += arr[j];
			}
		}
		m /= arr.Num();

		t += Monitization::PrecomputedTimeAspectScore(score1, score2, m);
	}
	avgscore1 /= Ad->positive_effects.Num();
	t /= Ad->positive_effects.Num();
	float k = 0;
	for (int i = 0; i < (int)Ad->negative_effects.Num(); i++) //Goes through all the negative side effects and manages them for us
	{
		if ((Ad->negative_effects[i].DiminishingReturn + 
			(Attributes[i].Delta * Ad->ComputeCompletionTime(this->Attributes[0].Value))) 
			> Attributes[Ad->negative_effects[i].type].Value)
				return 0;;
		k += Monitization::AutoWeightedScore_Negative(avgscore1, t, 
			Ad->negative_effects[i].DiminishingReturn + Attributes[Ad->negative_effects[i].type].Delta * Ad->ComputeCompletionTime(this->Attributes[0].Value),
			Attributes[Ad->negative_effects[i].type].Value);
	}
	k /= Ad->negative_effects.Num();
	return t * k;
};