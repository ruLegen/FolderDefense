// Fill out your copyright notice in the Description page of Project Settings.


#include "World/RoomHandler.h"

#include "DrawDebugHelpers.h"

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
	Room->Setup(RoomSize,RoomHeight);
	Room->FinishSpawning(FTransform());
	//FVector AroundSizes;
	//FVector Origin;
	//Room->GetExtendedBox(AroundSizes);
	//Room->GetCenter(Origin);
	//UE_LOG(LogTemp,Warning,TEXT("%s ,%s"),*AroundSizes.ToString(),*Origin.ToString());
	//DrawDebugBox(GetWorld(),Origin,AroundSizes,FColor::Red,true,4,0,12);
	//for (int i = 0; i < 100; ++i)
	//{
	//	FVector Position = Room->GetRandomPointInside();
	//	DrawDebugPoint(GetWorld(),Position,10,FColor::Blue,true,4,0);
	//}
}

// Called every frame
void ARoomHandler::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

}

