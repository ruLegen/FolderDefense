// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/FDMatchGameMode.h"

#include "FDGameInstance.h"
#include "GameFramework/PlayerStart.h"
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

AActor* AFDMatchGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	auto World = GetWorld();
	if(!World) return Super::ChoosePlayerStart_Implementation(Player);

	auto MatchPlayerState = Player->GetPlayerState<AFDMatchPlayerState>();
	if(!MatchPlayerState) return Super::ChoosePlayerStart_Implementation(Player);

	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(World,APlayerStart::StaticClass(),PlayerStarts);
	for (auto StartActor : PlayerStarts)
	{	auto PlayerStart = Cast<APlayerStart>(StartActor);
		if(!PlayerStart) continue;

		FName PlayerIndex(FString::FromInt(MatchPlayerState->GetTeam()));		
		if(PlayerStart->PlayerStartTag.IsEqual(PlayerIndex))
		{
			return PlayerStart;
		}
	}
	
	return Super::FindPlayerStart_Implementation(Player,IncomingName);
}

//AActor* AFDMatchGameMode::ChoosePlayerStart_Implementation(AController* Player)
//{
//	auto World = GetWorld();
//	if(!World) return Super::ChoosePlayerStart_Implementation(Player);
//
//	auto MatchPlayerState = Player->GetPlayerState<AFDMatchPlayerState>();
//	if(!MatchPlayerState) return Super::ChoosePlayerStart_Implementation(Player);
//
//	TArray<AActor*> PlayerStarts;
//	UGameplayStatics::GetAllActorsOfClass(World,APlayerStart::StaticClass(),PlayerStarts);
//	for (auto StartActor : PlayerStarts)
//	{	auto PlayerStart = Cast<APlayerStart>(StartActor);
//		if(!PlayerStart) continue;
//
//		FName PlayerIndex(FString::FromInt(MatchPlayerState->GetTeam()));		
//		if(PlayerStart->PlayerStartTag.IsEqual(PlayerIndex))
//		{
//			return PlayerStart;
//		}
//	}
//	
//	return Super::ChoosePlayerStart_Implementation(Player);
//
//}

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
	auto MatchPlayerState = NewPlayer->GetPlayerState<AFDMatchPlayerState>();
	MatchPlayerState->SetTeam(Players.Num());
	NewPlayerController->SelectFolder();
}
