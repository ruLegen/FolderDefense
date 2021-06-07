// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "File.generated.h"

class AFolder;
/**
 * 
 */
USTRUCT()

struct FFile
{
	GENERATED_BODY()
public:
	void Init(FString FileName,FString FolderPath);
	
	UPROPERTY(VisibleAnywhere)
	FString Name;

	UPROPERTY(VisibleAnywhere)
	FString Path;

	UPROPERTY(VisibleAnywhere)
	FString Type;

};
