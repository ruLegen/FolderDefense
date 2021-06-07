// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDLobbyHUD.h"

#include "Blueprint/UserWidget.h"
#include "GameModes/FDLobbyGameModeBase.h"
#include "GameStates/FDLobbyGameStateBase.h"
#include "UI/Widgets/FDLobbyWidget.h"

void AFDLobbyHUD::BeginPlay()
{
	Super::BeginPlay();
	auto LobbyState = GetWorld()->GetGameState<AFDLobbyGameStateBase>();
	if (!LobbyState) return;
	
	LobbyState->OnPlayerUpdate.AddUObject(this, &AFDLobbyHUD::OnPlayersUpdate);
	LobbyWidget = Cast<UFDLobbyWidget>(CreateWidget<UUserWidget>(GetWorld(), LobbyWidgetClass));
	if (!LobbyWidget) return;

	LobbyWidget->AddToViewport();

	if (GetLocalRole() == ROLE_Authority) {
		TArray<FString> NameArray;

		NameArray.Add(GetOwningPlayerController()->GetName());
		LobbyWidget->UpdatePlayers(NameArray);
	}
}

void AFDLobbyHUD::OnPlayersUpdate(TArray<FString> Players)
{
	if (!LobbyWidget) return;
	LobbyWidget->UpdatePlayers(Players);

	if (GetLocalRole() == ROLE_Authority) {
		auto GameMode = GetWorld()->GetAuthGameMode<AFDLobbyGameModeBase>();
		if (!GameMode) return;

		if (GameMode->GetNumOfConnectedPlayers() >= GameMode->GetMaxPlayers()) 
		{
			LobbyWidget->SetButtonVisibility(true);
		}
		else 
		{
			LobbyWidget->SetButtonVisibility(false);
		}
	}
}
