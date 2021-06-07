// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FDBaseWeapon.h"
#include "GameFramework/Actor.h"
#include "FDRifleWeapon.generated.h"
//class USTUWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
UCLASS()
class FOLDERDEFENSE_API AFDRifleWeapon : public AFDBaseWeapon
{
	GENERATED_BODY()
	
public:	
	AFDRifleWeapon();
	UFUNCTION(BlueprintCallable)
	void MakeShot();
	UFUNCTION(BlueprintCallable,Server,Unreliable)
	void Hit( FVector Vector,  FVector TraceEnd);
	virtual void StartFire()  override;
	virtual void StopFire()  override;

protected:
	virtual void BeginPlay() override;
	
	//UPROPERTY(EditAnywhere, Category = "VFX")
	//USTUWeaponFXComponent *WeaponFXComponent;
	
	UPROPERTY()
	UNiagaraComponent *MuzzzleFXComponent;
	void InitMuzzleFX();
	void SetMuzzleVisibility(bool state);
	UFUNCTION(NetMulticast,Reliable)
	void ApplyDamage(AActor* HitActor,APlayerController* PlayerController,ACharacter* Player);
};
