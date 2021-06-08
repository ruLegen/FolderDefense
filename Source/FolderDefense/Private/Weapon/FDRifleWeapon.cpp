// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FDRifleWeapon.h"

#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AFDRifleWeapon::AFDRifleWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	Inaccuracy = 0.1f;
	FireRate = 0.01f;
	bReplicates = true;
}

void AFDRifleWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty())
		return;

	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player)
		return;

	const auto PlayerController = Player->GetController<APlayerController>();
	if (!PlayerController)
		return;

	FVector ViewLocation;
	FRotator ViewRotation;
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

	FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
	FVector TraceStart = ViewLocation;

	FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), FMath::DegreesToRadians(Inaccuracy / 2));
	FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Blue, false, 3, 0, 1.0f);
	Hit(TraceStart, TraceEnd);
	/*
	 FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	DecreaseAmmo();

	if (HitResult.bBlockingHit)
	{
		//WeaponFXComponent->PlayImpactFX(HitResult);
		AActor *HitActor = HitResult.Actor.Get();
		UE_LOG(LogTemp,Warning,TEXT("HITED "));
		if (!HitActor)
			return;
		GEngine->AddOnScreenDebugMessage(0,1,FColor::Blue,"Hitted Actor");
		UE_LOG(LogTemp,Warning,TEXT("HITED ACTOR"));
		ApplyDamage(HitActor,PlayerController,Player);
		//HitActor->TakeDamage(10, {}, PlayerController, Player);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("NO HIT"));
		DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Blue, false, 3, 0, 1.0f);
	}
	*/
}

void AFDRifleWeapon::Hit_Implementation( FVector TraceStart, FVector TraceEnd)
{
	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);
	DecreaseAmmo();

	if (HitResult.bBlockingHit)
	{
		//WeaponFXComponent->PlayImpactFX(HitResult);
		AActor *HitActor = HitResult.Actor.Get();
		if (!HitActor)
			return;
		GEngine->AddOnScreenDebugMessage(0,1,FColor::Blue,"Hitted Actor");
		
		const auto Player = Cast<ACharacter>(GetOwner());
		if (!Player)
			return;
		const auto PlayerController = Player->GetController<APlayerController>();
		if (!PlayerController)
			return;
		GEngine->AddOnScreenDebugMessage(0,1,FColor::Blue,"Applyed Actor");
		HitActor->TakeDamage(10, {}, PlayerController, Player);
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("NO HIT"));
		//DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Blue, false, 3, 0, 1.0f);
	}
}


void AFDRifleWeapon::StartFire()
{
	GEngine->AddOnScreenDebugMessage(0,1,FColor::Blue,"Start Fire");
	//MakeShot();
	InitMuzzleFX();
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AFDRifleWeapon::MakeShot, FireRate, true);
}

void AFDRifleWeapon::StopFire()
{
	SetMuzzleVisibility(false);
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}


// Called when the game starts or when spawned
void AFDRifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFDRifleWeapon::InitMuzzleFX()
{
	if (!MuzzzleFXComponent)
	{
		MuzzzleFXComponent = SpawnMuzzleFX();
	}
	SetMuzzleVisibility(true);
}

void AFDRifleWeapon::SetMuzzleVisibility(bool Visible)
{
	if (MuzzzleFXComponent)
	{
		MuzzzleFXComponent->SetPaused(!Visible);
		MuzzzleFXComponent->SetPaused(Visible);
	}
}

void AFDRifleWeapon::ApplyDamage_Implementation(AActor* HitActor, APlayerController* PlayerController,ACharacter* Player)
{
	UE_LOG(LogTemp,Warning,TEXT("Applyed Damage"));
	GEngine->AddOnScreenDebugMessage(0,1,FColor::Blue,"Applyed Actor");
	HitActor->TakeDamage(10, {}, PlayerController, Player);
}
