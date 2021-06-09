// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UObject/NoExportTypes.h"
#include "EditorDirectories.h"
#include "DesktopPlatformModule.h"
#include "Misc/MessageDialog.h"
#include "Runtime/Core/Public/HAL/FileManagerGeneric.h"


#include "FileUtils.generated.h"

/**
 * 
 */
UCLASS()
class FOLDERDEFENSE_API UFileUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static bool OpenFileDialog(FString& Directory);
	/**
	*	Return array of directories in given directory FindInDirectory
	*/
	UFUNCTION(BlueprintCallable)
	static TArray<FString> FindDirectories(const FString SearchDirectory, bool bAbsolute);

	/**
	*	Return array of files in given directory FindInDirectory
	*/
	UFUNCTION(BlueprintCallable)
	static TArray<FString> FindFiles(const FString SearchDirectory,bool bAbsolute);


	UFUNCTION(BlueprintCallable,meta = (WorldContext = "WorldContextObject"))
	static int TestSomeFun(FString URL);
};
