#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class NAZIZOMBIES_API IInteractInterface
{
	GENERATED_BODY()

public:
	virtual void Interact(class ANaziZombiesCharacter* Character) = 0;

	//
	//virtual void DisplayInteractText(class ASurvivalHUD* HUD);
};
