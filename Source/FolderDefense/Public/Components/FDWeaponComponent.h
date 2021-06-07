// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Classes/Folder.h"
#include "Components/ActorComponent.h"
#include "FDWeaponComponent.generated.h"

class AFDBaseWeapon;
struct FWeaponUIData;
struct FAmmoData;
USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "Weapon")
	TSubclassOf<AFDBaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = "Weapon")
	UAnimMontage* ReloadAnimMontage;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FOLDERDEFENSE_API UFDWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFDWeaponComponent();
	void StartFire();
	void StopFire();
	void NextWeapon();
	void Reload();
	bool GetWeaponUIData(FWeaponUIData &WeaponUIData);
	bool GetWeaponData(FAmmoData &WeaponAmmoData);
	bool TryAddAmmo(TSubclassOf<AFDBaseWeapon> WeaponType, int32 Amount);
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName PrimaryWeaponAttachSocketName = "WeaponSocket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName SecondaryWeaponAttachSocketName = "ArmorySocket";

	UPROPERTY(EditDefaultsOnly, Replicated,Category = "Weapon")
	int32 CurrentWeaponIndex = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY(ReplicatedUsing= On_WeaponsReplicated)
	TArray<AFDBaseWeapon*> Weapons;
	UFUNCTION()
	void On_WeaponsReplicated();
	UPROPERTY(Replicated)
	AFDBaseWeapon *CurrentWeapon = nullptr;
	UPROPERTY()
	UAnimMontage *CurrentReloadAnimMontage = nullptr;

	UPROPERTY(Replicated)
	bool EquipAnimInProgress = false;
	UPROPERTY(Replicated)
	bool ReloadAnimInProgress = false;

	UPROPERTY(ReplicatedUsing=On_Test)
	FString test;
	
public:
	UFUNCTION()
	void On_Test()
	{
		if(GetOwner() && !GetOwner()->HasAuthority())
		{
			GEngine->AddOnScreenDebugMessage(0,3,FColor::Red,test);
		}
	}
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void SpawnWeapons();
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void UpdateTest(const FString& FolderJSON);
	
	void AttachWeaponToSocket(AFDBaseWeapon* Weapon, USceneComponent *Mesh, FName SocketName);
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void EquipWeapon(int32 WeaponIndex);
	void PlayAnim(UAnimMontage *AnimMontage);
	void InitAnimations();
	void OnEquipedFinished(USkeletalMeshComponent *SkeletalMesh);
	void OnReloadFinished(USkeletalMeshComponent *SkeletalMesh);
	bool CanFire();
	bool CanEquip();
	bool CanReload();
	void OnEmptyClip();
	void ChangeClip();
	template <class T> T *FindAnimNotifyByClass(UAnimSequenceBase* Animation);

};

template <class T>
T* UFDWeaponComponent::FindAnimNotifyByClass(UAnimSequenceBase* Animation)
{
	if (!Animation)
		return nullptr;
	
	auto const NotifyEvents = Animation->Notifies;
	for (auto NotifyEvent : NotifyEvents)
	{
		auto AnimMontage = Cast<T>(NotifyEvent.Notify);
		if (AnimMontage){return AnimMontage;}
	}
	return nullptr;
}
