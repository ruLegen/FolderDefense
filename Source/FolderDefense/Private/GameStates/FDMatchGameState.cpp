// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/FDMatchGameState.h"

#include "GameModes/FDMatchGameMode.h"
#include "Net/UnrealNetwork.h"
#include "PlayerStarts/FDPlayerStart.h"
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

void AFDMatchGameState::RespawnPlayer_Implementation(APlayerController*  PlayerController)
{
	if(!PlayerController) return;

	auto MatchWorld = GetWorld();
	if(!MatchWorld) return;

	auto MatchGameMode = MatchWorld->GetAuthGameMode<AFDMatchGameMode>();
	if(!MatchGameMode) return;
	
	MatchWorld->GetTimerManager().SetTimer(RespawnTimer,[PlayerController,MatchGameMode]()
	{
		PlayerController->UnPossess();
		MatchGameMode->RestartPlayer(PlayerController);
	},RespawnDelay,false);
}

/*
* Find PlayerStat where folder & file were killed
* Then Call NotifyKill for this player start;
*/

void AFDMatchGameState::NofifyKill_Implementation(const FFolder& Folder, AController* Controller, EEntityType Type, const FString& Name)
{
	for (auto Player : PlayerArray)
	{
		auto MatchPlayerState = Cast<AFDMatchPlayerState>(Player);
		if(!MatchPlayerState) return;

		if(MatchPlayerState->GetFolder() == Folder)
		{
			MatchPlayerState->NotifyKill(Controller,Type,Name);
		}
	}
	if(!GetWorld()) return;;

	auto MatchGameMode = GetWorld()->GetAuthGameMode<AFDMatchGameMode>();
	if(!MatchGameMode) return;

	for(auto Player : MatchGameMode->GetPlayerArray())
	{
		if(!Player) return;

		auto MatchPlayerState = Player->GetPlayerState<AFDMatchPlayerState>();
		if(!MatchPlayerState) return;
		Player->UpdateMatchState(MatchPlayerState->IsDefeat());

		if(MatchPlayerState->IsDefeat())
		{
			bIsGameOver = true;
			OnGameOver.Broadcast();
			UE_LOG(LogTemp,Warning,TEXT("Player %s defeat"),*Player->GetName());
		}
	}
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
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(World,APlayerStart::StaticClass(),PlayerStarts);
	
	int i = 0;
	for (auto PlayerState : PlayerArray)
	{
		auto MatchPlayerState = Cast<AFDMatchPlayerState>(PlayerState);
		if(!MatchPlayerState) continue;
		check(RoomClass);
		FTransform RoomTransform;
		for (auto PlayerStart : PlayerStarts)
		{
			auto StartPosition = Cast<APlayerStart>(PlayerStart);
			if(!PlayerStart) continue;

			FName PlayerIndex(FString::FromInt(MatchPlayerState->GetTeam()));		
			if(StartPosition->PlayerStartTag.IsEqual(PlayerIndex))
			{
				RoomTransform = StartPosition->GetTransform();
			}
		}
		
		auto RoomLocation = RoomTransform.GetLocation();
		RoomLocation.Z = 10;
		RoomLocation -= RoomTransform.GetRotation().Vector() * 900;
		RoomTransform.SetLocation(RoomLocation);
		
		auto RoomRotation = RoomTransform.GetRotation().Rotator();
		RoomRotation.Add(0,90,0);

		RoomTransform.SetRotation(RoomRotation.Quaternion());
		auto Room = World->SpawnActorDeferred<AProcedureRoom>(RoomClass,RoomTransform,MatchPlayerState->GetOwner());
		Room->Setup(4,1);
		Room->InitFolder(MatchPlayerState->GetFolder());
		Room->FinishSpawning(RoomTransform,true);
		i++;
	}
	
	if(!GetWorld()) return;

	auto gameMode = GetWorld()->GetAuthGameMode<AFDMatchGameMode>();
	if(!gameMode) return;
	
	gameMode->RestartRound(0);
}
