#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "FirearmComponent.generated.h"

UCLASS()
class NAZIZOMBIES_API UFirearmComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void Discharge(FVector WorldDirection);

protected:
	UPROPERTY(EditDefaultsOnly)
	class UFirearmData* FirearmData{nullptr};
};
