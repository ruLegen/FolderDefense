// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Classes/Folder.h"
#include "GameFramework/PlayerState.h"
#include "FDMatchPlayerState.generated.h"

enum class EEntityType : uint8;
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
	uint32 Team =-1;
	
	UPROPERTY(Replicated)
	uint32 Deaths;

	UPROPERTY(Replicated)
	uint32 Kills;

	UPROPERTY(Replicated)
	bool bIsDefeat;
	
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
	FFolder& GetFolder(){return Folder;}
	UFUNCTION(Server, reliable)
	void UpdatePlayerFolder(const FString& FolderJSON);
	
	UFUNCTION(Server, reliable)
	void NotifyKill(AController* KilledBy, EEntityType Type, const FString& Name);

	bool IsDefeat();
	
	UFUNCTION(Server, reliable)
	void SetTeam(uint32 WewTeam);
	int32 GetTeam() { return Team;};
};
