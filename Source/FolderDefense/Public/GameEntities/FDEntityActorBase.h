// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/TextRenderComponent.h"


#include "FDEntityActorBase.generated.h"

class UFDHealthComponent;
USTRUCT()
struct FEntityData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	float DefaultHealth = 200;
};
UCLASS()
class FOLDERDEFENSE_API AFDEntityActorBase : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,Replicated)
	UFDHealthComponent * HealthComponent;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UTextRenderComponent* TextComponent;

	UPROPERTY(EditDefaultsOnly)
	FEntityData Data;
	
	UPROPERTY(EditDefaultsOnly)
	FString EntityName;

	AFDEntityActorBase();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	virtual void Tick(float DeltaTime) override;
	void OnDie(AController* KilledBy);
	public:
	void SetEntityName(const FString& EntityName);
	

};
