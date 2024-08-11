#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AmmunitionData.generated.h"

UCLASS()
class NAZIZOMBIES_API UAmmunitionData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Properties)
	float TravelSpeed{86600.0f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Properties)
	int32 Damage{35};
};
