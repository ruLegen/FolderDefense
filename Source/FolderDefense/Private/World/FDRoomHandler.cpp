// Fill out your copyright notice in the Description page of Project Settings.


#include "World/FDRoomHandler.h"

#include "DrawDebugHelpers.h"
#include "GameEntities/FDFolderActor.h"

// Sets default values
AFDRoomHandler::AFDRoomHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates  = true;
}

// Called when the game starts or when spawned
void AFDRoomHandler::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFDRoomHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFDRoomHandler::GenerateRoom(FVector Location)
{
	

	
}

