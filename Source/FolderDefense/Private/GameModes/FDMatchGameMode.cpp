// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/FDMatchGameMode.h"

#include "FDGameInstance.h"
#include "GameStates/FDMatchGameState.h"
#include "PlayerStates/FDMatchPlayerState.h"
#include "UI/FDMatchGameHUD.h"
#include "Weapon/FDBaseWeapon.h"

class UFDGameInstance;

AFDMatchGameMode::AFDMatchGameMode()
{
	PlayerControllerClass = AFDMatchPlayerController::StaticClass();
	GameStateClass = AFDMatchGameState::StaticClass();
	HUDClass = AFDMatchGameHUD::StaticClass();
	PlayerStateClass = AFDMatchPlayerState::StaticClass();
}

void AFDMatchGameMode::RestartRound(int32 NextRound)
{
	for (auto Player : Players)
	{
		Player->StartSpot = nullptr;
		RestartPlayer(Player);
	}
}

void AFDMatchGameMode::FinishGame()
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

void AFDMatchGameMode::StartPlay()
{
	Super::StartPlay();
}

void AFDMatchGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	HandleNewPLayer(NewPlayer);
}

void AFDMatchGameMode::HandleSeamlessTravelPlayer(AController*& NewPlayer)
{
	Super::HandleSeamlessTravelPlayer(NewPlayer);
	auto NewPlayerControler = Cast<APlayerController>(NewPlayer);
	if (!NewPlayerControler) return;
	HandleNewPLayer(NewPlayerControler);
}

void AFDMatchGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	AFDMatchPlayerController* NewPlayerController = Cast<AFDMatchPlayerController>(Exiting);
	if (!NewPlayerController) return;

	Players.Remove(NewPlayerController);
	Exiting->Destroy();
}

void AFDMatchGameMode::ClearWorld()
{
	RemoveAllActorsByClass<AFDBaseCharacter>();
	RemoveAllActorsByClass<AFDBaseWeapon>();
}

void AFDMatchGameMode::HandleNewPLayer(APlayerController* NewPlayer)
{
	AFDMatchPlayerController* NewPlayerController = Cast<AFDMatchPlayerController>(NewPlayer);
	if (!NewPlayerController) return;

	SetPause(NewPlayerController);
	Players.Add(NewPlayerController);
	NewPlayerController->SelectFolder();
	// 	auto CurrentGameState = GetWorld()->GetGameState<AFDMatchGameState>();
	// 	if (!CurrentGameState) return;
	//
	// 	CurrentGameState->StartRound();
}
