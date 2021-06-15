// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FDLobbyUserRowWidget.h"
#include "Blueprint/UserWidget.h"
#include "FDLobbyWidget.generated.h"

/**
 * 
 */
class UButton;
class UScrollBox;
class UTextBlock;
class UWDLobbyPlayerRowWidget;
class UHorizontalBox;

UCLASS()
class FOLDERDEFENSE_API UFDLobbyWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;

	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
	UPROPERTY(meta = (BindWidget))
	UScrollBox *PlayerScrollBox;


	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextPlayerNum;


	UPROPERTY(EditDefaultsOnly,BLueprintReadWrite, Category="Row")
	TSubclassOf< UFDLobbyUserRowWidget> RowClass;


	public:
	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void StopGame();
	void UpdatePlayers(TArray<FString> Players);
	void SetButtonVisibility(bool Visible);	
};
