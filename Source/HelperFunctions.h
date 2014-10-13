#include <vector>
#include <math.h>
#define _USE_MATH_DEFINES
#define M_PI           3.14159265358979323846

struct Ownership //For obvious reasons. We could maybe implement theft (?)
{
	bool InUse;
	int EntityID;
};
class Cost
{
public:
	Cost(short t, int D)
	{
		this->type = t;
		this->DiminishingReturn = D;
	}
	short type; //Does this event cost us health, cost us money, etc
	int DiminishingReturn; //How badly does taking this advertisement hurt us?
};
enum JobType //The job type our current entity has
{
	BLACKSMITH = 0,
	GUARD,
	COOK,
	MERCHANT,
	MINER,
	FARMER,
	NOBLE,
	KING,
};
enum EntityType //In order of how much the AI cares
{
	Player,
	NPC,
	Building, //These do not have AI, but the AI has records of them
	Faction,
	Nation,
};
enum GoalVarType //This is what an AI's goal can be based off of
{
	Health,
	Wealth,
	Hunger,
	Social,
	JobIncome,
	JobType,
	Greed,
	Happiness,
	Attraction,
};
enum PossibleEvents //This is what Entity 1 did to Entity 2
{
	Killed,
	StoleFrom,
	Broke,
	Damaged,
	Built,
	Fixed,
	Moved,
	Helped,
};
struct DataStruct //Contains new information for the AI. Machine learning has to be utilized. This occurs when 2 NPCs talk amongst themselves, or occurs for a faction when new information arrives in town
{
	int Entity1; //This entity (ie: Player, King, Noble, Friend, Building, Bridge)
	short Action; //did ___ (ie: Killed, Damaged, Broke, Stole From, etc)
	int Entity2; //To this other entity
	static int ReturnEvent(int type) //Returns -1 if the event is a bad thing to do, +1 if its a good thing.
	{
		switch (type) //We can add tons more if we wanted. Due to architecture, we could potentially have an infinite amount of actions
		{
		case Killed:
			return -1;
			break;
		case StoleFrom:
			return -1;
			break;
		case Broke:
			return -1;
			break;
		case Damaged:
			return -1;
			break;
		case Built:
			return 1;
			break;
		case Fixed:
			return 1;
			break;
		case Moved:
			return 1;
			break;
		case Helped:
			return 1;
			break;
		}
	}
};
struct Opinion
{
public:
	char FeelingTowards; //0 - 4, I dislike it. 5 - 9, I like it
	int index; //What is my opinion against? (Index of the object in a world object manager)
	int Recent; //How recent was this event? The higher this number, the more recent it was. 

	std::vector<DataStruct> *CauseAndEffect; //What are my opinions based on?
protected:
	int ReturnRecent()
	{
		return Recent;
	}
	int ReturnIndex()
	{
		return index;
	}
	short ReturnFeelings()
	{
		return (short)FeelingTowards;
	}
public:
	static bool SortRecent(Opinion i, Opinion j) //Used for sorting
	{
		return (i.ReturnRecent() < j.ReturnRecent());
	}
	static bool SortIndex(Opinion i, Opinion j) //Used for sorting
	{
		return (i.ReturnIndex() < j.ReturnIndex());
	}
	static bool SortFeelings(Opinion i, Opinion j) //Used for sorting
	{
		return (i.ReturnFeelings() < j.ReturnFeelings());
	}

};
static int binarySearch(std::vector<Opinion> arr, int value, int min, int max){
	int pos = -1;

	while (max >= min && pos == -1) {
		int mid = min + (max - min) / 2;

		if (arr[mid].index == value){
			pos = mid;
		}
		else if (arr[mid].index < value){
			min = mid + 1;
		}
		else if (arr[mid].index > value){
			max = mid - 1;
		}

	}
	return pos;
};
static int binarySearch(std::vector<float> arr, int value, int min, int max){
	int pos = -1;

	while (max >= min && pos == -1) {
		int mid = min + (max - min) / 2;

		if (arr[mid] == value){
			pos = mid;
		}
		else if (arr[mid] < value){
			min = mid + 1;
		}
		else if (arr[mid] > value){
			max = mid - 1;
		}

	}
	return pos;
};
struct Monitization //Contains a set of helper functions for assisting the entity to figure out what helps it the most in its current eco state
{
	/*
	Erf positive is use for tasks with heigh weights. Tanh is slightly lower weighted than Erf. XPlusAbsX is the lowest ranked
	*/
	static float Attune(float input) //Is an increase from 30% to 60% worth just as much as an increase from 60% to 90%?
	{
		return 10 / input;
	};
	static float TimeAspect(float inputscore, int Delta, int EntityValue) //How much time is this process going to take? Are we going to die completing it?
	{
		inputscore *= Delta; //How much food do we need during this time?
		inputscore = 1 - (inputscore / EntityValue); //The more food it takes to complete a task, the lower timeaspect becomes
		return inputscore;
	};
	static float CostAttune_Erf_Positive(float inputscore, int Cost, int EntityValue) //This function is a sigmoid graph. At 0 cost, this function returns 1. At max cost, this function returns near 0
	{

		if (Cost > EntityValue)
			return -1;
		float var = (((float)Cost / (float)EntityValue) * 4) - 2; //This is what we feed into Error Function (http://mathworld.wolfram.com/Erf.html)
		var = (float)erf((M_PI / 2) * var);
		var += 1;
		var = 1 - var;

		//The higher the cost goes, the closer this value gets to 0. It never becomes negative. Look at the wolfram link for more information. All that is done here is compressions and scaling. (Just so the value returned is (0,1])
		return var * inputscore;
	}
	static float CostAttune_Tanh_Positive(float inputscore, int Cost, int EntityValue)
	{
		if (Cost > EntityValue)
			return -1;
		float var = (((float)Cost / (float)EntityValue) * 4) - 2;

		var = tanh(var);
		var += 1;
		var = 1 - var;

		return var;
	}
	static float CostAttune_XPlusAbsX_Positive(float inputscore, int Cost, int EntityValue)
	{
		if (Cost > EntityValue)
			return -1;
		float var = (((float)Cost / (float)EntityValue) * 4) - 2;

		var = var / (1 + abs(var));
		var += 1;
		var = 1 - var;

		return var;
	}
	static float AutoWeightedScore_Negative(int L, float inputscore, int Cost, int EntityValue) //L = score1 (The value we're trying to lerp) This is also quite overpowered
	{
		if (L <= 20) //We're only using Erf
		{
			return CostAttune_Erf_Positive(inputscore, Cost, EntityValue);
		}
		else if (L <= 30)
		{
			int t = 40 - L; //Distance from the 50 point
			t = (10 - t) / 10; //The further the number is from 50, the smaller this gets
			return (t * CostAttune_Tanh_Positive(inputscore, Cost, EntityValue)) + ((1 - t) * CostAttune_Erf_Positive(inputscore, Cost, EntityValue));
		}
		else if (L <= 50)
		{
			return CostAttune_Tanh_Positive(inputscore, Cost, EntityValue);
		}
		else if (L <= 60)
		{
			int  t = L - 40; //Distance from 50
			t = (20 - (L - 40)) / 20;
			return (t * CostAttune_Erf_Positive(inputscore, Cost, EntityValue)) + ((1 - t) * CostAttune_XPlusAbsX_Positive(inputscore, Cost, EntityValue));
		}
		return CostAttune_XPlusAbsX_Positive(inputscore, Cost, EntityValue);
	}
	static float StaticWeightedScore_Negative(int L, float inputscore, int Cost, int EntityValue)
	{
		if (L< 20) //It needs to be weighed highly
			return Monitization::CostAttune_Erf_Positive(inputscore, Cost, EntityValue);
		else if (L < 50) //Weighed in a medium matter
			return Monitization::CostAttune_Tanh_Positive(inputscore, Cost, EntityValue);
		else //Unweighed
			return Monitization::CostAttune_XPlusAbsX_Positive(inputscore, Cost, EntityValue);
	}
	//Feel free to add any more models you want :D We max out at about 40 models or so, so no need to hold back

	static float Score(int input1, int input2, int Delta, int EntityValue, float Completion) //Provides your base score. This is just an upper level for Attune and TimeAspect. Does nothing differently than managing attune and time aspect individually
	{
		float temp = Monitization::Attune((float)input1) - Monitization::Attune((float)input2);
		temp *= Completion;
		return TimeAspect(temp, Delta, EntityValue);
	}
	static float PrecomputedTimeAspectScore(int input1, int input2, float Pre)
	{
		float temp = Monitization::Attune((float)input1) - Monitization::Attune((float)input2);
		return temp * Pre;
	}
};