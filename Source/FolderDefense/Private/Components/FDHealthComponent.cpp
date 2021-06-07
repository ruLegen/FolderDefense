// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FDHealthComponent.h"

#include "Net/UnrealNetwork.h"

UFDHealthComponent::UFDHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void UFDHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	SetIsReplicated(true);
	check(MaxHealth > 0); 
	SetHealth(MaxHealth);
	OnHealthChanged.Broadcast(Health);
	AActor *Owner = GetOwner();
	if (Owner)
		Owner->OnTakeAnyDamage.AddDynamic(this, &UFDHealthComponent::OnAnyDamage);

	
}

void UFDHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UFDHealthComponent, Health);
}

void UFDHealthComponent::OnAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (IsDead()) return;

	SetHealth(Health -Damage);
	GEngine->AddOnScreenDebugMessage(0,1,FColor::Blue,FString::SanitizeFloat(Health));

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}


bool UFDHealthComponent::TryAddHealth(int32 Amount)
{
	if (Health == MaxHealth || IsDead()) return false;
	SetHealth(Health + Amount);
	return true;
}


void UFDHealthComponent::Heal_Implementation()
{
	SetHealth(Health+ HealAmount);
	if (Health >= MaxHealth)
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHanlde);
}

void UFDHealthComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp<float>(NewHealth, 0, MaxHealth);
	OnHealthChanged.Broadcast(NewHealth);
}


