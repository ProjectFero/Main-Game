#include "Fero/Fero.h"
#include <vector>
enum WeaponMaterial
{
	Copper = 0,
	Bronze,
	Iron,
	Steel,
	Mithril,
	Orichalcum,
	Obsidean,
	Black_Steel,
	Feromine,
};
enum AnimationToBeUsed
{
	StrafeLeft,
	StrafeRight,
	Lunge,
	Charge,
	BackStep,
};
struct RPG_Stat
{
private:
	int Level;
	int AttackChain, DefenseChain, HealChain;
	float NeededTimeDifference;
public:
	std::vector<float> StatsDefault;
	std::vector<float> StatsBuff;
	std::vector<int> StatsPts;
	RPG_Stat()
		: StatsDefault(5), StatsBuff(5), StatsPts(5)
	{
		Default_Stat(); LoadStats();
	};
	enum STAT
	{
		Attack = 0,
		Defense = 1,
		Support = 2,
		Dexterity = 3,
		Wisdom = 4,
	};
	void Default_Stat()
	{
		StatsDefault.at(0) = (.70f); //Index 0 = Base Attack - Percent Dmg
		StatsDefault.at(1) = (.70f); //Index 1 = Base Defense - Percent Defense
		StatsDefault.at(2) = (.70f); //Index 2 = Base Support - Percent Healing
		StatsDefault.at(3) = (.20f); //Index 3 = Base Dexterity - Percent Parrying
		StatsDefault.at(4) = (.1f); //Index 4 = Base Wisdom - Mana Requirement

		StatsBuff.assign(4, 0);
		StatsPts.assign(4, 0);
	};
	void LoadStats()
	{

	};
	void Buff_Condi(int index, int value)
	{
		StatsBuff.at(index) += value;
	};
	int GetPt(int index)
	{
		return StatsPts.at(index);
	};
	float GetStat(int index)
	{
		float r = StatsDefault.at(index) + StatsBuff.at(index) + (.01f) * StatsPts.at(index);
		return r;
	};
	void LevelUp(std::vector<int> Indicies, std::vector<int> Amount) //Incidies are the indexes that the player has selected to improve. Ammount is the point increase. AMOUNT MUST BE A WHOLE NUMBER
	{
		for (int i = 0; i < Indicies.size(); i++)
		{
			StatsPts.at(Indicies.at(i)) += Amount.at(i);
		}
		Level += 1;
	}
	float EffectiveDamage(float WeaponAttack, FVector Velocity, bool Feromine = false)
	{
		if (!Feromine)
			return WeaponAttack * AccountForSpeed(Velocity);
		else
			return WeaponAttack * (AccountForSpeed(Velocity) + .6f);
	}
	float EffectiveHit(float InputDamageToPlayer, float PlayerArmour, bool Feromine = false) //At this point, we know the block was successful
	{
		if (DefenseChain > 10)
			DefenseChain = 10;
		float TotalDefense = PlayerArmour  * (StatsDefault.at(1) + StatsBuff.at(1) + (DefenseChain * 0.02f));
		if (Feromine)
			TotalDefense *= 1.6f;

		return pow(InputDamageToPlayer, 2) / TotalDefense;
	};
	bool CorrectBlock(float RNG, float timedifference) //RNG needs to be a value from 0 - 1. It can be calculated from the players attack value if need be. Time difference is the difference between when the player rose the shield, and when the enemy attacked them
	{
		if (NeededTimeDifference < timedifference) //Blocking failed
		if (RNG > StatsDefault.at(STAT::Dexterity) + StatsBuff.at(STAT::Dexterity)) //Chain lost
		{
			AttackChain = 0;
			DefenseChain = 0;
			HealChain = 0;
			return false;
		}
		//Otherwise the blocking is sucessful
		DefenseChain += 1;
		return true;
	};
	float ReturnShieldDmg(float InputDamageToPlayer, float PlayerArmour, float RNG, float timedifference, bool Feromine = false)
	{
		if (CorrectBlock(RNG, timedifference))
			return EffectiveHit(InputDamageToPlayer * (1 - (StatsDefault.at(STAT::Dexterity) + StatsBuff.at(STAT::Dexterity))), PlayerArmour, Feromine);
		else
			EffectiveHit(InputDamageToPlayer, PlayerArmour, Feromine);
	};
private:
	float AccountForSpeed(FVector Velocity) //The faster the player is traveling, the higher their DPS, and the higher their defense. This accounts for that variable, as well as incorporating buffs.
	{
		AttackChain += 1;
		if ((Velocity.Size() == 0) || (AttackChain > 10))
			AttackChain = 10;

		if (Velocity.Size() < 10)
			return (float)((.2f * (sqrt(abs(Velocity.Size()) / 10))) + (StatsDefault.at(0) + StatsBuff.at(0)) + (AttackChain * 0.02f));
		else
			return .2f + (StatsDefault.at(0) + StatsBuff.at(0));

	}

};
