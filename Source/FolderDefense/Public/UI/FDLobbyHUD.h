// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FDLobbyHUD.generated.h"

class UFDLobbyWidget;
/**
 * 
 */
UCLASS()
class FOLDERDEFENSE_API AFDLobbyHUD : public AHUD
{
	GENERATED_BODY()
	protected:
	virtual void BeginPlay() override;
	UPROPERTY()
	UFDLobbyWidget* LobbyWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> LobbyWidgetClass;

	UFUNCTION()
		void OnPlayersUpdate(TArray<FString> Players);
};
