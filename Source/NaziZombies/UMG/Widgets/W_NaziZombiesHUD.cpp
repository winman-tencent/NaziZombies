#include "W_NaziZombiesHUD.h"

#include "Animation/WidgetAnimation.h"

void UW_NaziZombiesHUD::DisplayHitMarker()
{
	if (IsValid(FadeHitMarker) == false)
		return;

	PlayAnimation(FadeHitMarker);
}
