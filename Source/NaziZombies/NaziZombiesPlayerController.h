#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NaziZombiesPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class NAZIZOMBIES_API ANaziZombiesPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

protected:
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void SetupHUD();

protected:
	UPROPERTY()
	class ANaziZombiesHUD* HUDRef{nullptr};
};
