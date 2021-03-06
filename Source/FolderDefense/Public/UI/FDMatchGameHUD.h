// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FDMatchGameHUD.generated.h"

class UFDMatchGameOverWidget;
UENUM(BlueprintType)
enum class EHUDState :uint8
{
	NONE,
	ROUND_END,
	GAMEOVER,
	PAUSE,
	MAX
};

UCLASS()
class FOLDERDEFENSE_API AFDMatchGameHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UPROPERTY()
	UFDMatchGameOverWidget* GameOverWidget;
public:
	
	//UFUNCTION(Client,Reliable)
	void UpdatePlayerMatchResult(bool bIsDefeat);
	
	UFUNCTION()
	void SetState(EHUDState State);

	EHUDState GetHUDState() { return HUDState; };
	void InitAll();
	void HideAll();
private:
	EHUDState HUDState = EHUDState::NONE;
	TMap<EHUDState, UUserWidget*> HUDWidgetMap;
	

	void SetStateVisibility(EHUDState State);
	

	
	UFUNCTION()
	void OnExitButtonPressed();
};
