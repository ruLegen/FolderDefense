// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FDBaseWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnClipEmptySignature)

class USkeletalMeshComponent;
class UNiagaraComponent;
class UNiagaraSystem;
USTRUCT(BlueprintType)
struct FWeaponUIData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "UI")
	UTexture2D* MainIcon;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "UI")
	UTexture2D *CrosshairIcon;
};


USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Ammo")
	int32 Bullets;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Ammo")
	int32 Clips;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Ammo")
	bool IsInfinity;
};

UCLASS()
class FOLDERDEFENSE_API AFDBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AFDBaseWeapon();
	FOnClipEmptySignature OnEmpty;
	void ChangeClip();
	bool CanReload();
	bool IsFullAmmo();
	FWeaponUIData GetUIData() const {return WeaponUIData;};
	FAmmoData GetWeaponData() const {return CurrentAmmoData;};
	protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnyWhere,BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Components")
	float FireRate = 0.1f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Components")
	float Inaccuracy = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Components")
	float TraceMaxDistance = 1500;

	UPROPERTY(EditAnyWhere, Category = "Ammunition")
	FAmmoData DefaultsAmmoData{20,5,false};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	FWeaponUIData WeaponUIData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
	UNiagaraSystem *MuzzleFX;

	FTimerHandle FireTimerHandle;
	FName MuzzleSocketName = "MuzzleSocket";
	
	UNiagaraComponent* SpawnMuzzleFX();
	private:
	UPROPERTY(EditAnyWhere, Replicated, Category = "Ammunition")
	FAmmoData CurrentAmmoData;

	public:
	virtual void StartFire();
	virtual void StopFire();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FVector GetMuzzleWorldLocation() const;
	APlayerController *GetPlayerController() const;
	bool GetPlayerViewPoint(FVector &ViewLocation, FRotator &ViewRotation) const;
	bool GetTraceData(FVector &TraceStart, FVector &TraceEnd);

	UFUNCTION(BlueprintCallable)
	void MakeHit(FHitResult &HitResult, FVector &TraceStart, FVector &TraceEnd);

	UFUNCTION(Server,Reliable)
	void DecreaseAmmo();
	bool IsAmmoEmpty();
	bool IsClipEmpty();
	void LogAmmo();
	bool TryAddAmmo(int32 Amount);
};
