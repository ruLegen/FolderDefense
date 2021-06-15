// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/FDLobbyGameModeBase.h"

#include "FDGameInstance.h"
#include "GameStates/FDLobbyGameStateBase.h"
#include "PlayerControllers/FDLobbyPlayerController.h"
#include "UI/FDLobbyHUD.h"

AFDLobbyGameModeBase::AFDLobbyGameModeBase()
{
	HUDClass = AFDLobbyHUD::StaticClass();
	PlayerControllerClass = AFDLobbyPlayerController::StaticClass();
	GameStateClass = AFDLobbyGameStateBase::StaticClass();
	bUseSeamlessTravel = true;
}

void AFDLobbyGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

}

APlayerController* AFDLobbyGameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal,const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	if (Players.Num() >= MaxPlayer) {
		ErrorMessage = "No more slots in this lobby";
		return nullptr;
	}
	else
		return Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

}

void AFDLobbyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);


	AFDLobbyPlayerController* Controller = Cast<AFDLobbyPlayerController>(NewPlayer);
	if (!Controller) return;
	Players.Add(Controller);

	auto LobbyGameState = GetWorld()->GetGameState<AFDLobbyGameStateBase>();
	if (!LobbyGameState) return;

	LobbyGameState->AddPlayer(Controller->GetName());
}

void AFDLobbyGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AFDLobbyPlayerController* Controller = Cast<AFDLobbyPlayerController>(Exiting);
	if (!Controller) return;
	Players.Remove(Controller);

	auto LobbyGameState = GetWorld()->GetGameState<AFDLobbyGameStateBase>();
	if (!LobbyGameState) return;
	LobbyGameState->RemovePlayer(Controller->GetName());
}

void AFDLobbyGameModeBase::FinishGame()
{
	for (auto Player : Players)
	{
		Player->Disconnect();			// Tell all clients to disconnect
	}
	// if we are lsiten server. Disconnect local player controller
	auto GameInstance = GetGameInstance<UFDGameInstance>();
	if (GameInstance)
	{
		GameInstance->DestroySession();
	}
	GetWorld()->ServerTravel(UFDGameInstance::MainMenuLevel, true, true);
}
