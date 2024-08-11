#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NaziZombies/Interfaces/InteractInterface.h"
#include "Firearm.generated.h"

UCLASS()
class NAZIZOMBIES_API AFirearm : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
	AFirearm();

public:
	UFUNCTION()
	void Discharge(FVector WorldDirection);

	virtual void Interact(ANaziZombiesCharacter* Character) override;

public:
	UFUNCTION()
	class UFirearmComponent* GetFirearmComponent() const {return FirearmComponent;}

protected:
	UPROPERTY(VisibleAnywhere)
	class UFirearmComponent* FirearmComponent{nullptr};
};
