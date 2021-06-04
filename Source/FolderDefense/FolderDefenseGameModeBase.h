// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FolderDefenseGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class FOLDERDEFENSE_API AFolderDefenseGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int i;
};
