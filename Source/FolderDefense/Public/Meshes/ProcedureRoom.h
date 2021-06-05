// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcedureRoom.generated.h"


UCLASS()
class FOLDERDEFENSE_API AProcedureRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcedureRoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform & Transform) override;
	UPROPERTY(EditAnywhere)
	int WallCount = 1;
	UPROPERTY(EditAnywhere)
	int WallHeight = 1;

	UPROPERTY(EditDefaultsOnly)
	UInstancedStaticMeshComponent* WallInstanceHolder;
	UPROPERTY(EditDefaultsOnly)
	UInstancedStaticMeshComponent* FloorInstanceHolder;

	UPROPERTY(EditAnywhere)
	UStaticMesh* WallStaticMesh;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh* FloorStaticMesh;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void GenerateRoom();

	UFUNCTION(BlueprintCallable)
	void Setup(int _WallCount);
};
