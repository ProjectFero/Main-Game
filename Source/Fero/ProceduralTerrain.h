

#pragma once
#include "Tickable.h"
#include "Object.h"
#include "ProceduralTerrain.generated.h"

/**
 * 
 */
UCLASS()
class FERO_API UProceduralTerrain : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Terrain)
	bool IsPerPixel;

	UFUNCTION(BlueprintCallable, Category = Terrain)
	void GenerateTerrain(class ALandscape *landscape);
};
