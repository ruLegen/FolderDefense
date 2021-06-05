// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Folder.generated.h"

class UFile;

/**
 * 
 */
UCLASS()
class FOLDERDEFENSE_API UFolder : public UObject
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FString Name;

	UPROPERTY(VisibleAnywhere)
	FString Path;

	UPROPERTY(VisibleAnywhere)
	int Depth = 0;

	UPROPERTY()
	bool bIsDirectoriesInitialized = false;

	UPROPERTY()
	bool bIsFilesInitialized = false;

	TWeakObjectPtr<UFolder> Parent;
	
	UPROPERTY(VisibleAnywhere)
	TArray<UFolder*> Directories;

	UPROPERTY(VisibleAnywhere)
	TArray<UFile*> Files;


public:
	UFolder(){}
	UFolder(const FString& Path)
	{
		this->Path = Path; 
	}

	UFUNCTION()
	void Init(const FString& FolderPath,UFolder* ParentFolder, int CurrentDepth);
	UFUNCTION(BlueprintCallable)
	FString GetPath() const {return Path;}
	
	UFUNCTION(BlueprintCallable)
	UFolder* GetParent() const 	{ return Parent.Get();  }

	UFUNCTION(BlueprintCallable)
	TArray<UFolder*>& GetDirectories() { return Directories;	}

	UFUNCTION(BlueprintCallable)
	const TArray<UFile*>& GetFiles() const	{	return Files;	}

	UFUNCTION(BlueprintCallable)
	static UFolder* CreateInstace(FString FolderPath,int DepthLimit);

protected:
	UFUNCTION()
	bool InitializeDirectories();			// Fill	Directories
	UFUNCTION()
	bool InitializeFiles();					// Fill	Files

};
