// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Character/FDBaseCharacter.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllers/FDMatchPlayerController.h"

#include "FDMatchGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FOLDERDEFENSE_API AFDMatchGameMode : public AGameMode
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly)
	int32 MaxPlayers = 2;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxRounds = 4;
	
	int32 CurrentRound = 0;

	TArray<AFDMatchPlayerController*> Players;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AFDBaseCharacter>> PlayerPawns;		// Array of PlayerSpawsn for each round

	virtual void StartPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void HandleSeamlessTravelPlayer(AController*& C) override;

	virtual void Logout(AController* Exiting) override;
	//virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	//virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	//virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	//virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;
	// Delete Unpossessed Pawn and Its guns
	UFUNCTION()
	void ClearWorld();
	
	// Handle Players that come by SeamlessTravell
	UFUNCTION()
	void HandleNewPLayer(APlayerController* NewPlayer);

	
	// Generate PlayerStarts
	//void ResetSpawnPoints();


	template <class T >
	void RemoveAllActorsByClass();
	public:
	AFDMatchGameMode();
	
	int32 GetMaxRounds() { return MaxRounds; }
	int32 GetMaxPlayers() { return MaxPlayers; }
	
	UFUNCTION()
	void RestartRound(int32 NextRound);
	
	//UFUNCTION()
	//void ResetPlayerStarts();
	
	//UFUNCTION()
	//void ResetPickups();
	
	UFUNCTION()
	void FinishGame();
};

template<class T>
void AFDMatchGameMode::RemoveAllActorsByClass()
{
	TArray<AActor*> ActorList;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), T::StaticClass(), ActorList);
	for (auto Actor : ActorList)
	{
		Actor->Destroy();
	}

};
