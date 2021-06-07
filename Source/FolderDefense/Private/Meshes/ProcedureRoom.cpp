// Fill out your copyright notice in the Description page of Project Settings.


#include "Meshes/ProcedureRoom.h"


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
	for (int h = 1; h <= WallHeight; h++)	/// Generates Height Walls
	{
		for (int i = 0; i <= 3; i++)		// Rotate Wall Generator
		{
			for (int j = 0; j < WallCount; j++)	// Walls
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

void AProcedureRoom::Setup(int _WallCount,int _RoomHeight)
{
	this->WallCount = _WallCount;
	this->WallHeight = _RoomHeight;
}

bool AProcedureRoom::GetExtendedBox(FVector& Output)
{
	if(!WallStaticMesh || !FloorStaticMesh)
		return false;
	auto WallSize = WallStaticMesh->GetBounds().BoxExtent;
	Output = FVector(WallSize.Y*WallCount,WallSize.Y*WallCount,WallSize.Z*WallHeight);
	return true;
}

void AProcedureRoom::GetCenter(FVector& Output)
{
	if(!WallStaticMesh || !FloorStaticMesh)
		return;
	auto WallSize = WallStaticMesh->GetBounds().BoxExtent;
	auto Location = GetActorLocation();
	Output = FVector(Location.X-(WallSize.Y*WallCount),Location.Y +(WallSize.Y*WallCount),Location.Z+(WallSize.Z*WallHeight));
}

FVector AProcedureRoom::GetRandomPointInside()
{
	if(!WallStaticMesh || !FloorStaticMesh)
		return GetActorLocation();
	auto WallSize = WallStaticMesh->GetBounds().BoxExtent;
	auto Location = GetActorLocation();
	return  FVector(Location.X-FMath::RandRange(0,(WallSize.Y*WallCount*2.0)),Location.Y +FMath::RandRange(0,(WallSize.Y*WallCount*2.0)),Location.Z);
}


// Called every frame
void AProcedureRoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}