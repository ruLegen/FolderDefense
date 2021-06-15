// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FDMainMenuWidget.generated.h"

class FOnlineSessionSearchResult;
class UEditableTextBox;
class UWidgetSwitcher;
class UScrollBox;
class UButton;


UENUM()
enum class EMainMenuAction : uint8 {
	NONE,
	HOST_GAME,
	JOIN_GAME  ,
	MAX	
};
UCLASS()
class FOLDERDEFENSE_API UFDMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* HostButton;
 
	UPROPERTY(meta = (BindWidget))
	UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	UButton* BackToMainMenuButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RefreshButton;


	UPROPERTY(meta = (BindWidget))
	UButton* DisclaimerButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* DisclaimerToMainMenuButton;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* LayoutSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ServerListScrollBox;

	EMainMenuAction MenuAction = EMainMenuAction::NONE;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ServerRowClass;
	
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnHostGame();

	UFUNCTION()
	void OnJoinGame();

	UFUNCTION()
	void OnBackToMenu();

	UFUNCTION()
	void OnRefreshClicked();

	UFUNCTION()
	void OnDisclaimerClicked();

	void OnServerClicked(int32 Id);
	void OnRefreshed(TArray<FOnlineSessionSearchResult>& Servers);
	void UpdateServers(TArray<FOnlineSessionSearchResult>& Servers);

	UFUNCTION()
	void HostGame();
};
