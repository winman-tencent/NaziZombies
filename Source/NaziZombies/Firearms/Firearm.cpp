#include "Firearm.h"

#include "NaziZombies/FirearmComponent.h"
#include "NaziZombies/NaziZombiesCharacter.h"

AFirearm::AFirearm()
{
	PrimaryActorTick.bCanEverTick = true;

	FirearmComponent = CreateDefaultSubobject<UFirearmComponent>(TEXT("FirearmComponent"));
	SetRootComponent(FirearmComponent);

	FirearmComponent->SetCollisionProfileName("PhysicsActor");
	FirearmComponent->SetSimulatePhysics(true);
	FirearmComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void AFirearm::Discharge(FVector WorldDirection)
{
	FirearmComponent->Discharge(WorldDirection);
}

void AFirearm::Interact(ANaziZombiesCharacter* Character)
{
	Character->EquipFirearm(this);
}

