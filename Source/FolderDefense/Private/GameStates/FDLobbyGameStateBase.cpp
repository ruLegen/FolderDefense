// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/FDLobbyGameStateBase.h"

#include "Net/UnrealNetwork.h"

void AFDLobbyGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFDLobbyGameStateBase, PlayerNames);
}

void AFDLobbyGameStateBase::OnRep_PlayerNameUpdated()
{
	OnPlayerUpdate.Broadcast(PlayerNames);
}

void AFDLobbyGameStateBase::AddPlayer_Implementation(const FString& Player)
{
	PlayerNames.Add(Player);
	OnRep_PlayerNameUpdated();
}

void AFDLobbyGameStateBase::RemovePlayer_Implementation(const FString& Player)
{
	PlayerNames.Remove(Player);
	OnRep_PlayerNameUpdated();
}
