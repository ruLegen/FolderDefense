// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Classes/Folder.h"
#include "Components/TextRenderComponent.h"
#include "FDEntityActorBase.generated.h"

class UNiagaraSystem;
class UFDHealthComponent;

UENUM(BlueprintType)
enum class EEntityType :uint8
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

	UPROPERTY(EditDefaultsOnly,Replicated)
	FFolder ParentFolderClass;

	UPROPERTY(EditDefaultsOnly, replicated, Category = "VFX")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	UNiagaraSystem* NiagaraDieEffect;
	
	AFDEntityActorBase();


	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	EEntityType Type = EEntityType::FOLDER;
	UPROPERTY(Replicated)
	int32 Id = 0;

	UFUNCTION( NetMulticast, Reliable)
	void SetId(int32 NewId);

	UFUNCTION(Server, Reliable)
	void SetParentFolderClass(const FFolder& Folder);
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast,Reliable)
	void SpawnNiagaraEffect(UNiagaraSystem* NiagaraSystem,FLinearColor Color);
	
	void OnDie(AController* KilledBy);
	
	UFUNCTION(NetMulticast,Reliable)
	void SetEntityName(const FString& EName);
	
	UFUNCTION(NetMulticast,Reliable)
	void UpdateEnityName();
	
	UFUNCTION(NetMulticast,Reliable)
	void SetMaterialInstance(UMaterialInstance* MaterialInstance);
};
