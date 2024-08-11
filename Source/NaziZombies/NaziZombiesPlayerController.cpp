#include "NaziZombiesPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "UMG/NaziZombiesHUD.h"

void ANaziZombiesPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	SetupHUD();
}

void ANaziZombiesPlayerController::SetupHUD()
{
	if (IsValid(HUDRef))
		return;

	HUDRef = GetHUD<ANaziZombiesHUD>();
}
