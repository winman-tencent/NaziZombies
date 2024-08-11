#include "FirearmComponent.h"

#include "TraceProjectile.h"
#include "DataAssets/FirearmData.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"

void UFirearmComponent::Discharge(FVector WorldDirection)
{
	if (IsValid(FirearmData) == false)
		return;

	if (IsValid(FirearmData->AmmunitionClass) == false)
		return;

	if (IsValid(GetSocketByName("S_Muzzle")) == false)
		return;
	
	ATraceProjectile* Projectile = GetWorld()->SpawnActor<ATraceProjectile>(FirearmData->AmmunitionClass, GetSocketLocation("S_Muzzle"), GetSocketRotation("S_Muzzle"));
	Projectile->SetDirectionOfMotion(WorldDirection);

	if (IsValid(GetAttachParentActor()))
	{
		if (ACharacter* Character = Cast<ACharacter>(GetAttachParentActor()))
		{
			if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
			{
				Projectile->SetInstigator(PlayerController);
			}
		}
	}
}
