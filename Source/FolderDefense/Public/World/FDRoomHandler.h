// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Classes/Folder.h"
#include "GameEntities/FDEntityActorBase.h"
#include "GameFramework/Actor.h"
#include "Meshes/ProcedureRoom.h"

#include "FDRoomHandler.generated.h"

UCLASS()
class FOLDERDEFENSE_API AFDRoomHandler : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFDRoomHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AProcedureRoom> RoomClass;


public:
	
	UPROPERTY(EditAnywhere)
	int RoomSize = 10;

	UPROPERTY(EditAnywhere)
	int RoomHeight = 3;

	virtual void Tick(float DeltaTime) override;
	void GenerateRoom(FVector Location);
	
};
