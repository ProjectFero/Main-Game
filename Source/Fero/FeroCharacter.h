// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameFramework/SpringArmComponent.h"
#include "Entity/Inventory/PickUpItem.h"
#include "FeroCharacter.generated.h"

UCLASS(config=Game)
class AFeroCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TSubobjectPtr<class USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TSubobjectPtr<class UCameraComponent> FollowCamera;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float healthvalue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float manavalue;

protected:
	bool ActionKeyDown;
	void F_Press() { ActionKeyDown = true; };
	void F_Release() { ActionKeyDown = false; };
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	void CheckHealth()
	{
		if (healthvalue < 0)
			OnDeath();
		else
			alive = true;
	};

	virtual void OnDeath()
	{
		alive = false;
	};


	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	bool alive;
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
public:

	bool IsAlive()
	{
		return alive;
	};
	UFUNCTION(BlueprintCallable, Category = Pawn)
	void SetManaDelta(float delta);


	UFUNCTION(BlueprintCallable, Category = Pawn)
	void SetManaValue(float val);

	UFUNCTION(BlueprintCallable, Category = Pawn)
	void SetHealthDelta(float delta);


	UFUNCTION(BlueprintCallable, Category = Pawn)
	void SetHealthValue(float val);

	void Tick(float DeltaSeconds) override;

	void PickUpItem(APickUpItem *Item);

	UPROPERTY(EditAnywhere, Category = Inventory)
		TArray<APickUpItem*> ItemInventory;

	UFUNCTION(BlueprintPure, meta = (FriendlyName = "Get Inv", CompactNodeTitle = "GetInv", Keywords = "Get Player Inventory"), Category = Inv)
		TArray<APickUpItem*> GetCurrentInventory();


};

