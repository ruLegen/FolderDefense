// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Meshes/ProcedureRoom.h"

#include "RoomHandler.generated.h"

UCLASS()
class FOLDERDEFENSE_API ARoomHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	int RoomSize = 10;

	UPROPERTY(EditAnywhere)
	int RoomHeight = 3;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProcedureRoom> RoomClass;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
