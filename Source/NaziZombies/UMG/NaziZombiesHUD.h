#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NaziZombiesHUD.generated.h"

UCLASS()
class NAZIZOMBIES_API ANaziZombiesHUD : public AHUD
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void UpdateInteractText(FString String);

	UFUNCTION()
	void SetCrosshairVisible(bool Visible);

	UFUNCTION()
	void DisplayHitMarker();
	
public:
	UPROPERTY()
	class UW_NaziZombiesHUD* Widget{nullptr};

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UW_NaziZombiesHUD> WidgetClass{nullptr};
};
