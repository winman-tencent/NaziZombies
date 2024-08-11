#include "NaziZombiesCharacter.h"
#include "NaziZombiesProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FirearmComponent.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Firearms/Firearm.h"
#include "Interfaces/InteractInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/NaziZombiesHUD.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ANaziZombiesCharacter::ANaziZombiesCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void ANaziZombiesCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitHUDRef();
}

void ANaziZombiesCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TraceForInteractables();
}

void ANaziZombiesCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ANaziZombiesCharacter::Move);
		
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ANaziZombiesCharacter::Look);

		EnhancedInputComponent->BindAction(DischargeAction, ETriggerEvent::Triggered, this, &ANaziZombiesCharacter::DischargeFirearm);
		
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ANaziZombiesCharacter::TryInteract);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ANaziZombiesCharacter::InitHUDRef()
{
	if (IsValid(HUDRef))
		return;

	HUDRef = Cast<APlayerController>(GetController())->GetHUD<ANaziZombiesHUD>();
}


void ANaziZombiesCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ANaziZombiesCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ANaziZombiesCharacter::DischargeFirearm()
{
	if (IsValid(EquippedFirearm) == false)
		return;
	
	FVector TraceStart = FirstPersonCameraComponent->GetComponentLocation();
	FVector TraceEnd = TraceStart + FirstPersonCameraComponent->GetForwardVector() * 9999.0f;

	FVector MuzzleLocation = EquippedFirearm->GetFirearmComponent()->GetSocketLocation("S_Muzzle");
	
	FVector WorldDirection = (TraceEnd - MuzzleLocation).GetSafeNormal();
	
	FCollisionQueryParams TraceParams{};
	TraceParams.AddIgnoredActor(this);
	FHitResult HitResult{};
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
	{
		WorldDirection = (HitResult.Location - MuzzleLocation).GetSafeNormal();
	}
		
	EquippedFirearm->Discharge(WorldDirection);
}

void ANaziZombiesCharacter::EquipFirearm(AFirearm* Firearm)
{
	if (IsValid(EquippedFirearm))
	{
		EquippedFirearm->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquippedFirearm->SetActorEnableCollision(true);
		EquippedFirearm->GetFirearmComponent()->SetSimulatePhysics(true);
	}
	
	EquippedFirearm = Firearm;
	EquippedFirearm->GetFirearmComponent()->SetSimulatePhysics(false);
	EquippedFirearm->SetActorEnableCollision(false);
	EquippedFirearm->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "GripPoint");
}

void ANaziZombiesCharacter::TraceForInteractables()
{
	FVector TraceStart = FirstPersonCameraComponent->GetComponentLocation();
	FVector TraceEnd = TraceStart + FirstPersonCameraComponent->GetForwardVector() * InteractRange;
	
	FCollisionQueryParams TraceParams{};
	TraceParams.AddIgnoredActor(this);
	TArray<FHitResult> HitResults{};
	
	if (!GetWorld()->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
	{
		if (IsValid(HUDRef))
			HUDRef->SetCrosshairVisible(false);

		LastInteractable = nullptr;
		return;
	}

	TArray<AActor*> InteractableActors{};
	for(auto Result : HitResults)
	{
		if (auto Interactable = Cast<IInteractInterface>(Result.GetActor()))
		{
			InteractableActors.Add(Result.GetActor());
		}
	}

	if (InteractableActors.Num() <= 0)
	{
		if (IsValid(HUDRef))
			HUDRef->SetCrosshairVisible(false);

		LastInteractable = nullptr;
		return;
	}
	
	float ClosestDistance{};
	AActor* ClosestInteractableActor = UGameplayStatics::FindNearestActor(TraceStart, InteractableActors, ClosestDistance);
	IInteractInterface* ClosestInteractable = Cast<IInteractInterface>(ClosestInteractableActor);

	if (ClosestInteractable != LastInteractable)
	{
		if (IsValid(HUDRef))
			HUDRef->SetCrosshairVisible(true);

		LastInteractable = ClosestInteractable;
	}
}

void ANaziZombiesCharacter::TryInteract()
{
	if (LastInteractable == nullptr)
		return;

	LastInteractable->Interact(this);
}
