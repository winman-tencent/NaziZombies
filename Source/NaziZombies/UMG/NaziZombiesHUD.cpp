#include "NaziZombiesHUD.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Widgets/W_NaziZombiesHUD.h"

void ANaziZombiesHUD::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(WidgetClass) == false)
		return;

	Widget = CreateWidget<UW_NaziZombiesHUD>(GetOwningPlayerController(), WidgetClass);
	Widget->AddToViewport();
	SetCrosshairVisible(false);
}

void ANaziZombiesHUD::UpdateInteractText(FString String)
{
	if (IsValid(Widget) == false)
		return;
	
	if (IsValid(Widget->InteractText) == false)
		return;

	Widget->InteractText->SetText(FText::FromString(String));
}

void ANaziZombiesHUD::SetCrosshairVisible(bool Visible)
{
	if (IsValid(Widget) == false)
		return;
	
	if (IsValid(Widget->Crosshair) == false)
		return;

	Widget->Crosshair->SetVisibility(Visible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void ANaziZombiesHUD::DisplayHitMarker()
{
	if (IsValid(Widget) == false)
		return;

	Widget->DisplayHitMarker();
}
