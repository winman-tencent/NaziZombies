#include "TraceProjectile.h"

#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DataAssets/AmmunitionData.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "UMG/NaziZombiesHUD.h"

ATraceProjectile::ATraceProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Parent"));
	SetRootComponent(ArrowComponent);
	ArrowComponent->SetArrowSize(0.1f);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(ArrowComponent);
	Mesh->SetRelativeRotation({0, -90.0f, 0.0f});
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bReplicates = true;
	SetReplicatingMovement(true);
	bAlwaysRelevant = true;
	ArrowComponent->SetIsReplicated(true);
}

void ATraceProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ATraceProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);
	ForceNetRelevant();
	SetDirectionOfMotion(ArrowComponent->GetForwardVector());
}

void ATraceProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (Velocity.Length() > 10.0f)
		{
			FHitResult hitResult;
			FVector StartLineTrace = GetActorLocation();
			FVector EndLineTrace = (Velocity * DeltaTime) + StartLineTrace;
			FVector ForwardVector = EndLineTrace - StartLineTrace;	
			SetActorRotation(ForwardVector.Rotation());

			if (bDrawDebugTracer)
				DrawDebugLine(GetWorld(), StartLineTrace, EndLineTrace, FColor::Purple, false, DebugTracerLifetime, 0, DebugTracerWidth);
	
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(this);
 
			RotationPitch = GetActorRotation().Pitch * -1;

			FCollisionQueryParams Params{};
			Params.AddIgnoredActors(ActorsToIngore);
			if(GetWorld()->LineTraceSingleByChannel(hitResult, StartLineTrace, EndLineTrace, ECC_GameTraceChannel1, Params))
			{
				if (auto hitActor = hitResult.GetActor())
				{
					if (ACharacter* SomeEntity = Cast<ACharacter>(hitActor))
					{
						USkeletalMeshComponent* EntityMesh = SomeEntity->GetMesh();
						FName ParentBoneName = hitResult.BoneName;
						FString ParentBoneString = ParentBoneName.ToString();
						
						while (ParentBoneString != "None")
						{
							if (ParentBoneString == "head" ||		// Head
								ParentBoneString == "neck_01" ||	// Neck
								ParentBoneString == "spine_03" ||	// Chest
								ParentBoneString == "spine_01" ||	// Stomach
								ParentBoneString == "upperarm_r" ||	// Right Arm
								ParentBoneString == "upperarm_l" ||	// Left Arm
								ParentBoneString == "hand_r" ||		// Right Hand
								ParentBoneString == "hand_l" ||		// Left Hand
								ParentBoneString == "thigh_r" ||	// Right Leg
								ParentBoneString == "thigh_l" ||	// Left Leg
								ParentBoneString == "foot_r" ||		// Right Foot
								ParentBoneString == "foot_l"		// Left Foot
								) 
							{
								//SomeEntity->TakeDamage(ParentBoneString, this);
								UE_LOG(LogTemp, Warning, TEXT("Bullet Hit %s in the %s!"), *hitActor->GetName(), *ParentBoneName.ToString());
								break;
							}
							else
							{
								ParentBoneName = EntityMesh->GetParentBone(ParentBoneName);
								ParentBoneString = ParentBoneName.ToString();
							}
						}
					}

					if (IsValid(InstigatorController))
					{
						if (ANaziZombiesHUD* HUD = InstigatorController->GetHUD<ANaziZombiesHUD>())
							HUD->DisplayHitMarker();
					}
				}

				if (IsValid(BulletImpactVFX))
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BulletImpactVFX, hitResult.Location, hitResult.Normal.Rotation());

				Destroy();
			}
	
			BulletTime += DeltaTime;
			SetActorLocation(EndLineTrace);
			if(!(120.0f >= RotationPitch && RotationPitch <= 70.0f))
			{
				RotationPitch = {};
			}
			Velocity.Z += Gravity;

			if(BulletTime > BulletLifetime)
			{
				Destroy();
				
			}
		}
		else
		{
			Mesh->SetCollisionProfileName("PhysicsActor");
			Mesh->SetSimulatePhysics(true);
		}
	}
}

void ATraceProjectile::SetDirectionOfMotion(FVector NewDirection)
{
	if (IsValid(AmmunitionData) == false)
		return;
	
	Velocity = NewDirection.GetSafeNormal() * AmmunitionData->TravelSpeed;
}

void ATraceProjectile::SetInstigator(APlayerController* PlayerController)
{
	InstigatorController = PlayerController;
}

float ATraceProjectile::GetDamage() const
{
	if (IsValid(AmmunitionData) == false)
		return 0.0f;
	
	return AmmunitionData->Damage;
}

UAmmunitionData* ATraceProjectile::GetAmmunitionData() const
{
	return AmmunitionData;
}
