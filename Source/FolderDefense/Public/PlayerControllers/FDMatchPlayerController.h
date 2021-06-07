// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/FDMatchGameHUD.h"

#include "FDMatchPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FOLDERDEFENSE_API AFDMatchPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void OnStartRound();
	void OnEndRound();
	void OnRestartRound();
	void OnGameOver();
public:
	
	AFDMatchPlayerController();
	

	UFUNCTION(Client,reliable)
	void SelectFolder();

	UFUNCTION(Client,reliable)
	void Disconnect();
	UFUNCTION(Server,Reliable)
	void SendFolderToServer(const FString& JSONFolder);


private:
	EHUDState PrevHUDState = EHUDState::NONE;

	void OnPausePressed();
};
