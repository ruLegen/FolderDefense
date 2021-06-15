// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FDWeaponComponent.h"

#include "FileUtils.h"
#include "JsonObjectConverter.h"
#include "Animations/FDEquipFinishedAnimNotify.h"
#include "Animations/FDReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/FDBaseWeapon.h"

UFDWeaponComponent::UFDWeaponComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
	
}

void UFDWeaponComponent::StartFire()
{
	
	if (CanFire())
		CurrentWeapon->StartFire();
}

void UFDWeaponComponent::StopFire()
{
	if (!CurrentWeapon)
		return;
	CurrentWeapon->StopFire();
}

void UFDWeaponComponent::NextWeapon()
{
}

void UFDWeaponComponent::Reload()
{
	ChangeClip();
}

bool UFDWeaponComponent::GetWeaponUIData(FWeaponUIData& WeaponUIData)
{
	if (CurrentWeapon)
	{
		WeaponUIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool UFDWeaponComponent::GetWeaponData(FAmmoData& WeaponAmmoData)
{
	if (CurrentWeapon)
	{
		WeaponAmmoData = CurrentWeapon->GetWeaponData();
		return true;
	}
	return false;
}

bool UFDWeaponComponent::TryAddAmmo(TSubclassOf<AFDBaseWeapon> WeaponType, int32 Amount)
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryAddAmmo(Amount);
		}
	}
	return false;
}


// Called when the game starts
void UFDWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
	InitAnimations();
}

void UFDWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// CurrentWeapon = nullptr;
	// for (auto Weapon : Weapons)
	// {
	// 	Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	// 	Weapon->Destroy();
	// }
	// Weapons.Empty();
	Super::EndPlay(EndPlayReason);
}

void UFDWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFDWeaponComponent, Weapons);
	DOREPLIFETIME(UFDWeaponComponent, CurrentWeaponIndex);
	DOREPLIFETIME(UFDWeaponComponent, EquipAnimInProgress);
	DOREPLIFETIME(UFDWeaponComponent, ReloadAnimInProgress);
	DOREPLIFETIME(UFDWeaponComponent, CurrentWeapon);
	DOREPLIFETIME(UFDWeaponComponent, test);
}

void UFDWeaponComponent::SpawnWeapons_Implementation()
{
	if (!GetWorld())
		return;

	ACharacter *Character = Cast<ACharacter>(GetOwner());
	if (!Character)
		return;
	for (auto WeaponClass : WeaponData)
	{
		auto Weapon = GetWorld()->SpawnActor<AFDBaseWeapon>(WeaponClass.WeaponClass);
		if (!Weapon)
			continue;
		Weapon->SetOwner(GetOwner());
		Weapon->OnEmpty.AddUObject(this, &UFDWeaponComponent::OnEmptyClip);
		Weapons.Add(Weapon);
		AttachWeaponToSocket(Weapon, Character->GetMesh(), SecondaryWeaponAttachSocketName);
	}
}

void UFDWeaponComponent::On_WeaponsReplicated()
{
	;
}

void UFDWeaponComponent::UpdateTest_Implementation(const FString& FolderJSON)
{
	;
	
}

void UFDWeaponComponent::AttachWeaponToSocket(AFDBaseWeapon* Weapon, USceneComponent* Mesh, FName SocketName)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(Mesh, AttachmentRules, SocketName);
	Weapon->SetOwner(GetOwner());
}

void UFDWeaponComponent::EquipWeapon_Implementation(int32 WeaponIndex)
{
	if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
		return;

	ACharacter *Character = Cast<ACharacter>(GetOwner());
	StopFire();
	if (!Character)
		return;
	if (CurrentWeapon) // If we have already gun in hand
		{
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), SecondaryWeaponAttachSocketName);
		}

	CurrentWeapon = Weapons[WeaponIndex];
	const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData &Data) { //
		return Data.WeaponClass == CurrentWeapon->GetClass();								 //
	});
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), PrimaryWeaponAttachSocketName);
	PlayAnim(EquipAnimMontage);
	EquipAnimInProgress = true;
}

void UFDWeaponComponent::PlayAnim_Implementation(UAnimMontage* AnimMontage)
{
	ACharacter *Character = Cast<ACharacter>(GetOwner());
	if (!Character)
		return;
	Character->PlayAnimMontage(AnimMontage);
}



void UFDWeaponComponent::InitAnimations()
{
	auto EquipFinishedNotify = FindAnimNotifyByClass<UFDEquipFinishedAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotify.AddUObject(this, &UFDWeaponComponent::OnEquipedFinished);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Forgotten setup equip anim"))
	}
	for (auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishedNotify =
			FindAnimNotifyByClass<UFDReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishedNotify)
		{
			UE_LOG(LogTemp, Error, TEXT("Forgotten setup reload anim"))
		}
		ReloadFinishedNotify->OnNotify.AddUObject(this, &UFDWeaponComponent::OnReloadFinished);
	}
}

void UFDWeaponComponent::OnEquipedFinished(USkeletalMeshComponent* SkeletalMesh)
{
	ACharacter *Character = Cast<ACharacter>(GetOwner());
	if (!Character || SkeletalMesh != Character->GetMesh())
		return;
	EquipAnimInProgress = false;
	UE_LOG(LogTemp, Display, TEXT("Finieshed"));
}

void UFDWeaponComponent::OnReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	ACharacter *Character = Cast<ACharacter>(GetOwner());
	if (!Character || SkeletalMesh != Character->GetMesh())
		return;
	ReloadAnimInProgress = false;
	UE_LOG(LogTemp, Display, TEXT("Finieshed"));
}

bool UFDWeaponComponent::CanFire()
{
	return CurrentWeapon != nullptr && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UFDWeaponComponent::CanEquip()
{
	return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UFDWeaponComponent::CanReload()
{
	return CurrentWeapon != nullptr && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

void UFDWeaponComponent::OnEmptyClip_Implementation()
{
	ChangeClip();
}



void UFDWeaponComponent::ChangeClip_Implementation()
{
	if (!CanReload())
		return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	PlayAnim(CurrentReloadAnimMontage);
	ReloadAnimInProgress = true;
}

