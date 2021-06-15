// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "FDPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class FOLDERDEFENSE_API AFDPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Multiplayer")
	uint32 TeamNumber = 0;
};
