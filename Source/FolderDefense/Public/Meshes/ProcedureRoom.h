// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Classes/Folder.h"
#include "GameEntities/FDFolderActor.h"
#include "GameFramework/Actor.h"
#include "ProcedureRoom.generated.h"
UENUM(BlueprintType)
enum class EFileType :uint8
{
	TEXT,
	AUDIO,
	IMAGE,
	UNKNOWN,
	MAX
};

class AFDEntityActorBase;
UCLASS()
class FOLDERDEFENSE_API AProcedureRoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcedureRoom();
	
	UPROPERTY(EditAnywhere,Replicated)
	int WallCount = 1;
	
	UPROPERTY(EditAnywhere,Replicated)
	int WallHeight = 1;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnConstruction(const FTransform & Transform) override;
	

	UPROPERTY(EditDefaultsOnly)
	UInstancedStaticMeshComponent* WallInstanceHolder;
	UPROPERTY(EditDefaultsOnly)
	UInstancedStaticMeshComponent* FloorInstanceHolder;

	UPROPERTY(EditAnywhere)
	UStaticMesh* WallStaticMesh;
	
	UPROPERTY(EditAnywhere)
	UStaticMesh* FloorStaticMesh;


	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AFDEntityActorBase> FolderClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AFDEntityActorBase> FileClass;
	
	UPROPERTY(EditAnywhere,Replicated)
	TArray<AFDEntityActorBase*> Folders;

	UPROPERTY(EditAnywhere,Replicated)
	TArray<AFDEntityActorBase*> Files;

	UPROPERTY(EditAnywhere,Replicated)
	FFolder Folder;

	UPROPERTY(EditAnywhere)
	TArray<UMaterialInstance*> FolderMaterials;								// Array of Folder  Material
	
	UPROPERTY(EditAnywhere)
	TMap<EFileType,UMaterialInstance*> FileMaterials;						// Map File extension with Material

	UPROPERTY(EditAnywhere)
	UMaterialInstance* DefaultFileMaterial;									// Default File Material
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable,NetMulticast,Reliable)
	void GenerateRoom();
	
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void GenerateFolders();
	
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void GenerateFiles();
	
	UFUNCTION(BlueprintCallable,Server,Reliable)
	void Setup(int _WallCount,int _RoomHeight);

	UFUNCTION(BlueprintCallable)
	bool GetExtendedBox(FVector& Output);

	UFUNCTION(BlueprintCallable)
	void GetCenter(FVector& Output);

	UFUNCTION(BlueprintCallable)
	FVector GetRandomPointInside();

	UFUNCTION(Server, Reliable)
	void InitFolder(FFolder FolderStruct);
	
	EFileType GetFileType(const FString& CS);
};
