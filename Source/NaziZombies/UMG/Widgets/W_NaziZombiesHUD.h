#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_NaziZombiesHUD.generated.h"

UCLASS()
class NAZIZOMBIES_API UW_NaziZombiesHUD : public UUserWidget
{
	friend class ANaziZombiesHUD;
	GENERATED_BODY()

protected:
	UFUNCTION()
	void DisplayHitMarker();
	
protected:
	UPROPERTY(meta = (BindWidgetOptional))
	class UImage* Crosshair{nullptr};

	UPROPERTY(meta = (BindWidgetOptional))
	class UImage* HitMarker{nullptr};

	UPROPERTY(Transient, meta=(BindWidgetAnimOptional))
	class UWidgetAnimation* FadeHitMarker{nullptr};

	UPROPERTY(meta = (BindWidgetOptional))
	class UTextBlock* InteractText{nullptr};
};
