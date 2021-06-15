// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FDRifleWeapon.h"

#include "DrawDebugHelpers.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFDRifleWeapon::AFDRifleWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	Inaccuracy = 0.1f;
	FireRate = 0.01f;
	bReplicates = true;
}

void AFDRifleWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFDRifleWeapon, MuzzzleFXComponent);
	DOREPLIFETIME(AFDRifleWeapon, bIsMuzzleVisible);
}

void AFDRifleWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty())
	{
		SetMuzzleVisibility(false);
		return;
	}

	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player)
		return;

	const auto PlayerController = Player->GetController<APlayerController>();
	if (!PlayerController)
		return;
	SetMuzzleVisibility(true);
	FVector ViewLocation;
	FRotator ViewRotation;
	PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	FTransform SocketTransform = WeaponMesh->GetSocketTransform(MuzzleSocketName);
	FVector TraceStart = ViewLocation;

	FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), FMath::DegreesToRadians(Inaccuracy / 2));
	FVector TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	DrawDebugLine(GetWorld(), SocketTransform.GetLocation(), TraceEnd, FColor::Blue, false, 0.1, 0, 1.0f);
	PlayCameraShake();
	Hit(TraceStart, TraceEnd);

	AController* playerController = Cast<AController>(GetOwner());
	if(!playerController) return;
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

void AFDRifleWeapon::On_MuzzleUpdated()
{
	if (MuzzzleFXComponent)
	{
		//MuzzzleFXComponent->SetPa(!Visible);
		MuzzzleFXComponent->SetVisibility(bIsMuzzleVisible);
	}
}

void AFDRifleWeapon::SpawnImpactFX_Implementation(FVector Location, FVector Normal)
{
	if(!HitFX || !GetWorld()) return;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),HitFX,Location,Normal.Rotation());
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

		SpawnImpactFX(HitResult.Location,HitResult.Normal);
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
	InitMuzzleFX();

}

void AFDRifleWeapon::PlayCameraShake_Implementation()
{
	const auto Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller) return;

	if (CameraShake)
		Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void AFDRifleWeapon::InitMuzzleFX()
{
	if (!MuzzzleFXComponent)
	{
		MuzzzleFXComponent = SpawnMuzzleFX();
		MuzzzleFXComponent->SetVisibility(false);
		
	}
	//SetMuzzleVisibility(false);
}

void AFDRifleWeapon::SetMuzzleVisibility_Implementation(bool Visible)
{
	bIsMuzzleVisible = Visible;
	On_MuzzleUpdated();
}

void AFDRifleWeapon::ApplyDamage_Implementation(AActor* HitActor, APlayerController* PlayerController,ACharacter* Player)
{
	UE_LOG(LogTemp,Warning,TEXT("Applyed Damage"));
	GEngine->AddOnScreenDebugMessage(0,1,FColor::Blue,"Applyed Actor");
	HitActor->TakeDamage(10, {}, PlayerController, Player);
}
