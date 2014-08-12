

#pragma once
#include "Tickable.h"
#include "Object.h"
#include "Quest.generated.h"

/**
 *
 */
UCLASS()
class FERO_API UQuest : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
	FString QuestName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
	FString QuestDescription;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Quest)
	bool bActive;

	void SetActive(bool bActiveVal);

	UFUNCTION(BlueprintImplementableEvent, Category = Quest)
	void QuestBegin();

	UFUNCTION(BlueprintCallable, Category = Quest)
	void EndQuest();

	void Tick();

	UFUNCTION(BlueprintImplementableEvent, Category = Quest)
	void QuestTick();


	UFUNCTION(BlueprintImplementableEvent, Category = Quest)
	void QuestPickedItemUp();

};
