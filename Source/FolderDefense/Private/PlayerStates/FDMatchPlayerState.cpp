// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/FDMatchPlayerState.h"

#include "JsonObjectConverter.h"
#include "GameStates/FDMatchGameState.h"
#include "Net/UnrealNetwork.h"

void AFDMatchPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFDMatchPlayerState, Kills);
	DOREPLIFETIME(AFDMatchPlayerState, Deaths);
	DOREPLIFETIME(AFDMatchPlayerState, Folder);
}

void AFDMatchPlayerState::On_FolderChanges()
{
	;
}

void AFDMatchPlayerState::UpdatePlayerFolder_Implementation(const FString& FolderJSON)
{
	FFolder NewFolder;
	FJsonObjectConverter::JsonObjectStringToUStruct(FolderJSON, &NewFolder, 0, 0);
	this->Folder = NewFolder;
	On_FolderChanges();
	
	if(!GetWorld())return;

	auto MatchGameState = GetWorld()->GetGameState<AFDMatchGameState>();
	if(!MatchGameState) return;

	MatchGameState->Ready();
}

void AFDMatchPlayerState::AddKill_Implementation(int Num)
{
	Kills += Num;
}

void AFDMatchPlayerState::AddDeath_Implementation(int Num)
{
	Deaths += Num;
}

void AFDMatchPlayerState::ResetScore_Implementation()
{
	Kills = 0;
	Deaths = 0;
}

void AFDMatchPlayerState::ResetBonus_Implementation()
{
	// TODO ResetBonus
}
