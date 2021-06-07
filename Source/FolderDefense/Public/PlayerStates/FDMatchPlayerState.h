// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Classes/Folder.h"
#include "GameFramework/PlayerState.h"
#include "FDMatchPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FOLDERDEFENSE_API AFDMatchPlayerState : public APlayerState
{
	GENERATED_BODY()
protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UPROPERTY(Replicated)
	uint32 Deaths;

	UPROPERTY(Replicated)
	uint32 Kills;

	UPROPERTY(ReplicatedUsing=On_FolderChanges)
	FFolder Folder;
public:	
	UFUNCTION(Server, reliable)
	void AddKill(int Num);
	
	UFUNCTION(Server, reliable)
	void AddDeath(int Num);

	UFUNCTION(Server, reliable)
	void ResetScore();

	UFUNCTION(Server, reliable)
	void ResetBonus();

	UFUNCTION()
	void On_FolderChanges();
	
	uint32 GetKills() { return Kills; }
	uint32 GetDeaths() { return Deaths; }
	uint32 GetPoints() { return Kills*2 - Deaths; }

	UFUNCTION(Server, reliable)
	void UpdatePlayerFolder(const FString& FolderJSON);
};
