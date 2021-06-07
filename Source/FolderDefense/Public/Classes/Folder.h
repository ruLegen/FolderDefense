// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "File.h"
#include "UObject/NoExportTypes.h"
#include "Folder.generated.h"

class AFile;

/**
 * 
 */
USTRUCT()
struct FFolder 
{
	GENERATED_BODY()
public:
	FFolder();

private:
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

	UPROPERTY()
	TArray<FString> Directories;

	UPROPERTY()
	TArray<FFile> Files;


public:

	FFolder(const FString& Path)
	{
		this->Path = Path; 
	}

	void Init(const FString& FPath,int CurrentDepth);
	FString GetPath() const {return Path;}
	
	TArray<FString>& GetDirectories() { return Directories;	}

	const TArray<FFile>& GetFiles() const	{	return Files;	}

	static FFolder CreateInstace(FString FPath,int DepthLimit);

protected:
	bool InitializeDirectories();			// Fill	Directories
	bool InitializeFiles();					// Fill	Files

};
