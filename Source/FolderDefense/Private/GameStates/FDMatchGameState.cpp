// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/FDMatchGameState.h"

#include "GameModes/FDMatchGameMode.h"
#include "Net/UnrealNetwork.h"
#include "PlayerStates/FDMatchPlayerState.h"
#include "World/FDRoomHandler.h"

void AFDMatchGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFDMatchGameState, CurrentRound);
	DOREPLIFETIME(AFDMatchGameState, bIsGameOver);
	DOREPLIFETIME(AFDMatchGameState, EmptyPlayersCount);
	DOREPLIFETIME(AFDMatchGameState, DelayOnStartRound);
	DOREPLIFETIME(AFDMatchGameState, ReadyPlayerCount);
}

void AFDMatchGameState::OnRep_RoundChanged()
{
	OnRoundChanged.Broadcast();
	UpdateRoundStart();
	StartRound();
}

void AFDMatchGameState::OnRep_GameOver()
{
	if(bIsGameOver)
		OnGameOver.Broadcast();
}

void AFDMatchGameState::Ready_Implementation()
{
	ReadyPlayerCount++;
	if(ReadyPlayerCount >= MinReadyPlayerCount && GetWorld())
	{
		auto MatchGameMode = GetWorld()->GetAuthGameMode<AFDMatchGameMode>();
		MatchGameMode->ClearPause();
		StartRound();
	}
}

void AFDMatchGameState::UpdateRoundStart_Implementation()
{
	DelayOnStartRound = FMath::RandRange(3, 12);
}

void AFDMatchGameState::NotifyEmpty_Implementation()
{
	if (!GetWorld()) return;

	auto CurrentGameMode = GetWorld()->GetAuthGameMode<AFDMatchGameMode>();
	if (!CurrentGameMode) return;

	EmptyPlayersCount++;

	if (EmptyPlayersCount >= CurrentGameMode->GetMaxPlayers())
	{
		GetWorld()->GetTimerManager().SetTimer(Timer, [&]() {
			NextRound();
			}, DelayOnEmpty, false);
	}
}

void AFDMatchGameState::ResetMatch_Implementation()
{
	for (auto PlayerState : PlayerArray)
	{
		auto CastState = Cast<AFDMatchPlayerState>(PlayerState);
		if (CastState)
		{
			CastState->ResetScore();
			CastState->ResetBonus();
		}
	}

	auto CurrentGameMode = GetWorld()->GetAuthGameMode<AFDMatchGameMode>();
	if (!CurrentGameMode) return;

	CurrentRound = 0;
	CurrentGameMode->RestartRound(CurrentRound);
	OnRep_RoundChanged();			// Call implicitly to trigger event on Server
	bIsGameOver = false;
}

void AFDMatchGameState::EndRound_Implementation()
{
	OnRoundEnd.Broadcast();
}

void AFDMatchGameState::OnChangeRound_Implementation()
{
	if (!GetWorld()) return;

	auto CurrentGameMode = GetWorld()->GetAuthGameMode<AFDMatchGameMode>();
	if (!CurrentGameMode) return;

	CurrentRound++;
	EmptyPlayersCount = 0;			// All Players have bullets at start of round
	OnRep_RoundChanged();			// Call implicitly to trigger event on Server
	if (CurrentRound > CurrentGameMode->GetMaxRounds())
	{
		bIsGameOver = true;
		OnRep_GameOver();			// Call implicitly to trigger event on Server
	}
	else
	{
		CurrentGameMode->RestartRound(CurrentRound);
	}
}

void AFDMatchGameState::NextRound_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(Timer, [&]() {
		EndRound();
		GetWorld()->GetTimerManager().SetTimer(Timer, this, &AFDMatchGameState::OnChangeRound, DelayOnEndRound, false);
	}, DelayAfterDeath, false);
}

void AFDMatchGameState:: StartRound_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(Timer,
		[&]() {
			OnRoundStart.Broadcast();
			SpawnRooms();
			
		}, DelayOnStartRound, false);
}

void AFDMatchGameState::SpawnRooms_Implementation()
{
	auto World = GetWorld();
	if(!World) return;

	int i = 0;
	for (auto PlayerState : PlayerArray)
	{
		auto MatchPlayerState = Cast<AFDMatchPlayerState>(PlayerState);
		if(!MatchPlayerState) continue;
		check(RoomClass);
		auto Room = World->SpawnActorDeferred<AProcedureRoom>(RoomClass,FTransform(FVector(i*900,0,0)),MatchPlayerState->GetOwner());
		Room->Setup(2,1);
		Room->InitFolder(MatchPlayerState->GetFolder());
		Room->FinishSpawning(FTransform(FVector(i*900,0,0)));
		i++;
	}
}
