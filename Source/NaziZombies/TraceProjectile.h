#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TraceProjectile.generated.h"

UCLASS()
class NAZIZOMBIES_API ATraceProjectile : public AActor
{
	friend class UFirearmComponent;
	GENERATED_BODY()

	ATraceProjectile();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	void SetDirectionOfMotion(FVector NewDirection);
	void SetInstigator(class APlayerController* PlayerController);

public:
	float GetDamage() const;
	class UAmmunitionData* GetAmmunitionData() const;

protected:
	float BulletTime{0.0f};
	UPROPERTY(Replicated)
	FVector Velocity;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Gravity = -981.0f;
	float RotationPitch;
	UPROPERTY(EditDefaultsOnly)
	float BulletLifetime = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAmmunitionData* AmmunitionData{nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bDrawDebugTracer{true};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DebugTracerWidth{0.5f};
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DebugTracerLifetime{1.0f};

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> ActorsToIngore{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UParticleSystem* BulletImpactVFX{nullptr};

protected:
	UPROPERTY()
	class APlayerController* InstigatorController{nullptr};
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UArrowComponent* ArrowComponent{nullptr};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh{nullptr};
};
