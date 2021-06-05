// Fill out your copyright notice in the Description page of Project Settings.


#include "Meshes/ProcedureRoom.h"

#include <string>

// Sets default values
AProcedureRoom::AProcedureRoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	WallInstanceHolder = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall"));
	FloorInstanceHolder = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor"));
	RootComponent = WallInstanceHolder;
	FloorInstanceHolder->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AProcedureRoom::BeginPlay()
{
	Super::BeginPlay();
	if(WallStaticMesh && WallInstanceHolder)
		WallInstanceHolder->SetStaticMesh(WallStaticMesh);
	if(FloorStaticMesh && FloorInstanceHolder)
		FloorInstanceHolder->SetStaticMesh(FloorStaticMesh);
	GenerateRoom();
}

void AProcedureRoom::OnConstruction(const FTransform& Transform)
{
	GenerateRoom();
}


void AProcedureRoom::GenerateRoom()
{
	if(!WallStaticMesh || !FloorStaticMesh || !WallInstanceHolder )
		return;
	auto WallSize = WallStaticMesh->GetBounds().BoxExtent;
	int  RotationDeg = 0;
	FVector LastLocation;
	WallInstanceHolder->ClearInstances();
	for (int h = 0; h < WallHeight; h++)
	{
		for (int i = 0; i <= 3; i++)
		{
			for (int j = 0; j < WallCount; j++)
			{
				float YOffset = WallSize.Y *2 * j;
				auto DirectionVector = FVector(0,YOffset,0).RotateAngleAxis(RotationDeg,FVector::ZAxisVector);
			
				FTransform NewTransform;
				NewTransform.SetLocation(DirectionVector+LastLocation);
				FRotator Rotator = FRotator::MakeFromEuler(FVector(0,0,RotationDeg));
				//Rotator.Yaw = RotationDeg;
				NewTransform.SetRotation(Rotator.Quaternion());
				WallInstanceHolder->AddInstance(NewTransform);
			}
			FTransform LastInstanceTransform;
			float YOffset = WallSize.Y *2 ;

			auto DirectionVector = FVector(0,YOffset,0).RotateAngleAxis(RotationDeg,FVector::ZAxisVector);
			WallInstanceHolder->GetInstanceTransform(WallInstanceHolder->GetInstanceCount()-1,LastInstanceTransform);
			LastLocation = LastInstanceTransform.GetLocation() + DirectionVector;
			RotationDeg = (i+1) * 90;
		}
		float ZOffset = WallSize.Z * 2*h;
		LastLocation = FVector(0,0,ZOffset);
	}
	
	FloorInstanceHolder->ClearInstances();
	auto FloorSize = FloorStaticMesh->GetBounds().BoxExtent;
	FloorSize = FloorSize*2;
	int FloorXCount = FMath::CeilToInt(WallSize.Y*2*WallCount*1.0 / FloorSize.X);
	int FloorYCount = FMath::CeilToInt(WallSize.Y*2*WallCount*1.0 / FloorSize.Y);
	for (int i = 0; i < FloorXCount; ++i)
	{
		for (int j = 0; j < FloorYCount; ++j)
		{
			FTransform NewTransform;
			FVector NewLocation = FVector(-FloorSize.X*i,FloorSize.Y*j,0);
			NewTransform.SetLocation(NewLocation);
			FloorInstanceHolder->AddInstance(NewTransform);
		}
	}
	
}

void AProcedureRoom::Setup(int _WallCount)
{
	this->WallCount = _WallCount;
}


// Called every frame
void AProcedureRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}