// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FDLobbyGameStateBase.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FGameStatePlayerUpdateSignature, TArray<FString>)

UCLASS()
class FOLDERDEFENSE_API AFDLobbyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(ReplicatedUsing=OnRep_PlayerNameUpdated)
	TArray<FString> PlayerNames;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	protected:
	

	UFUNCTION()
	void OnRep_PlayerNameUpdated();
	public:
	FGameStatePlayerUpdateSignature OnPlayerUpdate;

	UFUNCTION(Server, reliable)
	void AddPlayer(const FString& Player);

	UFUNCTION(Server, reliable)
	void RemovePlayer(const FString& Player);

	TArray<FString>  GetPlayersArray() { return PlayerNames; }
};
