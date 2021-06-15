// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FDBaseWeapon.h"
#include "GameFramework/Actor.h"
#include "FDRifleWeapon.generated.h"
class UNiagaraComponent;
class UNiagaraSystem;
UCLASS()
class FOLDERDEFENSE_API AFDRifleWeapon : public AFDBaseWeapon
{
	GENERATED_BODY()
	
public:	
	AFDRifleWeapon();

	UPROPERTY(ReplicatedUsing=On_MuzzleUpdated)
	bool bIsMuzzleVisible = false;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void MakeShot();
	
	UFUNCTION(BlueprintCallable,Server,Unreliable)
	void Hit( FVector Vector,  FVector TraceEnd);

	UFUNCTION()
	void On_MuzzleUpdated();

	UFUNCTION(NetMulticast,Reliable)
	void SpawnImpactFX(FVector Location, FVector Normal);

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TSubclassOf<UCameraShakeBase> CameraShake;
	
	
	virtual void StartFire()  override;
	virtual void StopFire()  override;

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(Client, Reliable)
	void PlayCameraShake();

	UPROPERTY(Replicated)
	UParticleSystemComponent *MuzzzleFXComponent;
	
	
	void InitMuzzleFX();
	
	UFUNCTION(Server, Reliable)
	void SetMuzzleVisibility(bool state);
	
	UFUNCTION(NetMulticast,Reliable)
	void ApplyDamage(AActor* HitActor,APlayerController* PlayerController,ACharacter* Player);
};
