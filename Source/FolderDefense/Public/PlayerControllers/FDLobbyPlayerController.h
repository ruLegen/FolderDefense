// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FDLobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FOLDERDEFENSE_API AFDLobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel) override;
public:
	UFUNCTION(Client,Reliable)
	void Disconnect();
};
