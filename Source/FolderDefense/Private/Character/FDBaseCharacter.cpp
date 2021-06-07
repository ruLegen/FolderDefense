
#include "Character/FDBaseCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/FDHealthComponent.h"
#include "Components/FDWeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"

AFDBaseCharacter::AFDBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetMesh(),TEXT("b_head"));
	SpringArmComponent->bUsePawnControlRotation = true;

	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);
	//SpringArmComponent->SocketOffset= FVector(0.0,0,0.0f);
	
	HealthComponent = CreateDefaultSubobject<UFDHealthComponent>("Health Commponent");
	HealthComponent->OnHealthChanged.AddUObject(this, &AFDBaseCharacter::OnHealthChanged);


	WeaponComponent = CreateDefaultSubobject<UFDWeaponComponent>("Weapon Commponent");

	MaxRunSpeed = 3000;
	ImpulseMultiplier = 4;
}
void AFDBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	if(HasAuthority())
		HealthComponent->OnDeath.AddUObject(this, &AFDBaseCharacter::OnDeath);
	DefaultSpeed = GetMovementComponent()->GetMaxSpeed();
}

void AFDBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFDBaseCharacter, AimRotator);
	DOREPLIFETIME(AFDBaseCharacter, HealthComponent);
	DOREPLIFETIME(AFDBaseCharacter, WeaponComponent);
	DOREPLIFETIME(AFDBaseCharacter, IsWantToRun);
	DOREPLIFETIME(AFDBaseCharacter, MaxRunSpeed);
	DOREPLIFETIME(AFDBaseCharacter, DefaultSpeed);
}

void AFDBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsLocallyControlled())
		UpdateAimDirection(GetBaseAimRotation());
}

void AFDBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AFDBaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFDBaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &Super::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookAround", this, &Super::AddControllerYawInput);
	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &Super::Jump);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &AFDBaseCharacter::StartRun);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &AFDBaseCharacter::StopRun);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, WeaponComponent, &UFDWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, WeaponComponent, &UFDWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("NextWeapon", EInputEvent::IE_Pressed, WeaponComponent, &UFDWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, WeaponComponent, &UFDWeaponComponent::Reload);

}

void AFDBaseCharacter::UpdateAimDirection_Implementation(FRotator ClientAimRotation)
{
	AimRotator = ClientAimRotation;
}

void AFDBaseCharacter::MoveForward(float Input)
{
	AddMovementInput(GetActorForwardVector(), Input);
	IsMovingForward = Input > 0;
}

void AFDBaseCharacter::MoveRight(float Input)
{
	AddMovementInput(GetActorRightVector(), Input);
}
void AFDBaseCharacter::StartRun_Implementation()
{
	IsWantToRun = true;
	if (IsRunning())
	{
		UCharacterMovementComponent *MovementComponent = GetCharacterMovement();
		MovementComponent->MaxWalkSpeed = MaxRunSpeed;
		MovementComponent->AddImpulse(GetActorForwardVector() * ImpulseMultiplier, false);
	}
}
void AFDBaseCharacter::StopRun_Implementation()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	IsWantToRun = false;
}

void AFDBaseCharacter::OnHealthChanged(float health)
{
	//New health
}

bool AFDBaseCharacter::IsRunning()
{
	return IsWantToRun && IsMovingForward && GetVelocity().Size() > 0;
}

float AFDBaseCharacter::GetMovementDirection() const
{
	FVector ForwardVector = GetActorForwardVector();
	FVector VelocityVector = GetVelocity();
	FVector CrossProduct = FVector::CrossProduct(ForwardVector, VelocityVector);
	ForwardVector.Normalize();
	VelocityVector.Normalize();
	float angle = FMath::Acos(FVector::DotProduct(ForwardVector, VelocityVector));
	return FMath::RadiansToDegrees(angle) * FMath::Sign(CrossProduct.Z);
}

FRotator AFDBaseCharacter::GetAimDirection()
{
	const FVector AimDirWS = AimRotator.Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}


void AFDBaseCharacter::OnDeath()
{
	//PlayAnimMontage(DeathAnimationMontage);
	
	Kill();

}

void AFDBaseCharacter::Kill_Implementation()
{
	//GetController()->ChangeState(NAME_Spectating);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetEnablePhysicsBlending(true);

	GetCharacterMovement()->DisableMovement();
	WeaponComponent->StopFire();
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetLifeSpan(3);
	
	const auto PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;

	auto Spectator = GetWorld()->SpawnActor<ASpectatorPawn>();
	if (Spectator && HasAuthority())
	{
		Spectator->TeleportTo(GetActorLocation(), GetActorRotation(), false, true);
		PlayerController->Possess(Spectator);
	}
}

