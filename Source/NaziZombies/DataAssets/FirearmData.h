#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FirearmData.generated.h"

UCLASS()
class NAZIZOMBIES_API UFirearmData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ammunition)
	TSubclassOf<class ATraceProjectile> AmmunitionClass{nullptr};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
	USoundBase* FireSound{nullptr};
};
