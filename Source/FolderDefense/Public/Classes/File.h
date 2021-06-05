// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "File.generated.h"

class UFolder;
/**
 * 
 */
UCLASS()
class FOLDERDEFENSE_API UFile : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	FString Name;

	UPROPERTY(VisibleAnywhere)
	FString Path;

	UPROPERTY(VisibleAnywhere)
	FString Type;
	
	TWeakObjectPtr<UFolder> Parent;

public:
	void Init(FString FileName, UFolder * ParentFolder);
};
