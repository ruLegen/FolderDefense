// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "FDEntityActorBase.generated.h"

class UFDHealthComponent;

UENUM(BlueprintType)
enum class EEnityType :uint8
{
	FOLDER,
	FILE,
	MAX
};

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
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly,Replicated)
	UTextRenderComponent* TextComponent;

	UPROPERTY(EditDefaultsOnly)
	FEntityData Data;
	
	UPROPERTY(EditDefaultsOnly)
	FString EntityName;

	AFDEntityActorBase();


	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	EEnityType Type = EEnityType::FOLDER;
	virtual void Tick(float DeltaTime) override;
	void OnDie(AController* KilledBy);
	
	UFUNCTION(NetMulticast,Reliable)
	void SetEntityName(const FString& EName);
	
	UFUNCTION(NetMulticast,Reliable)
	void UpdateEnityName();
	UFUNCTION(NetMulticast,Reliable)
	void SetMaterialInstance(UMaterialInstance* MaterialInstance);
};
