// Fill out your copyright notice in the Description page of Project Settings.


#include "World/RoomHandler.h"

// Sets default values
ARoomHandler::ARoomHandler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARoomHandler::BeginPlay()
{
	Super::BeginPlay();
	auto World = GetWorld();
	if(!World) return;

	auto Room = World->SpawnActorDeferred<AProcedureRoom>(RoomClass,FTransform());
	Room->Setup(RoomSize);
	Room->FinishSpawning(FTransform());
	
}

// Called every frame
void ARoomHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

