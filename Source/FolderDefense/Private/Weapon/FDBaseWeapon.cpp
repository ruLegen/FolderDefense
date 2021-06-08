// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FDBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"

AFDBaseWeapon::AFDBaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Root");
	RootComponent = WeaponMesh;
	CurrentAmmoData = DefaultsAmmoData;
	bReplicates = true;
}

void AFDBaseWeapon::ChangeClip()
{
	CurrentAmmoData.Bullets = DefaultsAmmoData.Bullets;
	if (!CurrentAmmoData.IsInfinity)
	{
		if (CurrentAmmoData.Clips == 0)
		{
			UE_LOG(LogTemp, Display, TEXT("-----No More Clips for reload----"));
			return;
		}
		CurrentAmmoData.Clips--;
	}
	else
	{
		CurrentAmmoData.Clips = DefaultsAmmoData.Clips;
	}
}

bool AFDBaseWeapon::CanReload()
{
	return CurrentAmmoData.Bullets < DefaultsAmmoData.Bullets && CurrentAmmoData.Clips >0;
}

bool AFDBaseWeapon::IsFullAmmo()
{
	return CurrentAmmoData.Bullets == DefaultsAmmoData.Bullets && CurrentAmmoData.Clips == DefaultsAmmoData.Clips;
}

void AFDBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

UNiagaraComponent* AFDBaseWeapon::SpawnMuzzleFX()
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, WeaponMesh, MuzzleSocketName, FVector::ZeroVector,
												FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);

}

void AFDBaseWeapon::StartFire()
{
}

void AFDBaseWeapon::StopFire()
{
}

void AFDBaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFDBaseWeapon, CurrentAmmoData);
}

FVector AFDBaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

APlayerController* AFDBaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player)
		return nullptr;
	return Player->GetController<APlayerController>();
}

bool AFDBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto PlayerController = GetPlayerController();
	if (!PlayerController)
		return false;

	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	return true;
}

bool AFDBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation))
		return false;

	TraceStart = ViewLocation;
	FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}


void AFDBaseWeapon::MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd)
{
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bReturnPhysicalMaterial = true;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility,
										CollisionParams);

}


void AFDBaseWeapon::DecreaseAmmo_Implementation()
{
	if (CurrentAmmoData.Bullets == 0)
	{
		StopFire();
		return;
	};
	CurrentAmmoData.Bullets--;
	LogAmmo();
	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		OnEmpty.Broadcast();
	}
}

bool AFDBaseWeapon::IsAmmoEmpty()
{
	return !CurrentAmmoData.IsInfinity && CurrentAmmoData.Clips == 0 && IsClipEmpty();
}


bool AFDBaseWeapon::IsClipEmpty()
{
	return CurrentAmmoData.Bullets == 0;
}

void AFDBaseWeapon::LogAmmo()
{
	FString AmmoInfo = "Ammo " + FString::FromInt(CurrentAmmoData.Bullets) + " / ";
	AmmoInfo += CurrentAmmoData.IsInfinity?"Infinity":FString::FromInt(CurrentAmmoData.Clips);
	UE_LOG(LogTemp, Display, TEXT("%s"), *AmmoInfo);
}

bool AFDBaseWeapon::TryAddAmmo(int32 Amount)
{
	if (IsFullAmmo() || CurrentAmmoData.IsInfinity || Amount <=0) return false;
	if (IsAmmoEmpty())
	{
		CurrentAmmoData.Clips = FMath::Clamp(Amount, 0, DefaultsAmmoData.Clips+1);
		OnEmpty.Broadcast();
	}
	else if (CurrentAmmoData.Clips < DefaultsAmmoData.Clips)
	{
		const auto NextClipsAmount = CurrentAmmoData.Clips + Amount;
		if (DefaultsAmmoData.Clips - NextClipsAmount >= 0)
		{
			CurrentAmmoData.Clips = NextClipsAmount;
		}
		else
		{
			CurrentAmmoData.Clips = DefaultsAmmoData.Clips;
			CurrentAmmoData.Bullets = DefaultsAmmoData.Bullets;
		}
	}
	else
	{
		CurrentAmmoData.Bullets = DefaultsAmmoData.Bullets;
	}
	return true;
}

