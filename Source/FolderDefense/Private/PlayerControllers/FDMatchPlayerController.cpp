// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/FDMatchPlayerController.h"

#include "FileUtils.h"
#include "JsonObjectConverter.h"
#include "Character/FDBaseCharacter.h"
#include "Classes/Folder.h"
#include "GameStates/FDMatchGameState.h"
#include "PlayerStates/FDMatchPlayerState.h"
#include "UI/FDMatchGameHUD.h"

void AFDMatchPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
	if (!GetWorld()) return;


	auto CurrentGameState = GetWorld()->GetGameState<AFDMatchGameState>();
	if (CurrentGameState)
	{
		CurrentGameState->OnRoundStart.AddUObject(this, &AFDMatchPlayerController::OnStartRound);
		CurrentGameState->OnRoundChanged.AddUObject(this, &AFDMatchPlayerController::OnRestartRound);
		CurrentGameState->OnRoundEnd.AddUObject(this, &AFDMatchPlayerController::OnEndRound);
		CurrentGameState->OnGameOver.AddUObject(this, &AFDMatchPlayerController::OnGameOver);
	}
}

void AFDMatchPlayerController::OnStartRound()
{
	auto ControllingPawn = GetPawn<AFDBaseCharacter>();
	if (!ControllingPawn) return;

}

void AFDMatchPlayerController::OnEndRound()
{
	auto GameHUD = GetHUD<AFDMatchGameHUD>();
	if (!GameHUD) return;

	UE_LOG(LogTemp, Warning, TEXT("Round End"));
	GameHUD->SetState(EHUDState::ROUND_END);
}

void AFDMatchPlayerController::OnRestartRound()
{
	auto GameHUD = GetHUD<AFDMatchGameHUD>();
	if (!GameHUD) return;

	UE_LOG(LogTemp, Warning, TEXT("Round Changed"));
	GameHUD->SetState(EHUDState::NONE);
	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

void AFDMatchPlayerController::OnGameOver()
{
	auto GameHUD = GetHUD<AFDMatchGameHUD>();
	if (!GameHUD) return;

	UE_LOG(LogTemp, Warning, TEXT("Game Over"));
	GameHUD->SetState(EHUDState::GAMEOVER);

	DisableInput(this);
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

AFDMatchPlayerController::AFDMatchPlayerController()
{
 	bReplicates = true;
}

void AFDMatchPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &AFDMatchPlayerController::OnPausePressed);
}

void AFDMatchPlayerController::SelectFolder_Implementation()
{
	FString dir;
	bool bFolderSelected = false;
	FFolder Folder;
	while(!bFolderSelected)
	{
		bFolderSelected = UFileUtils::OpenFileDialog(dir);
		Folder = FFolder::CreateInstace(dir,0);
		int SelectedFolders = Folder.GetDirectories().Num();
		int SelectedFiles = Folder.GetDirectories().Num();
		bFolderSelected = bFolderSelected && (SelectedFolders > 3 || SelectedFiles > 3);
	}
	
	FString JSONPayload;
	FJsonObjectConverter::UStructToJsonObjectString(Folder, JSONPayload, 0, 0);

	auto World = GetWorld();
	if(!World) return;

	SendFolderToServer(JSONPayload);
}

void AFDMatchPlayerController::SendFolderToServer_Implementation(const FString& JSONFolder)
{
	auto MatchPlayerState = GetPlayerState<AFDMatchPlayerState>();
	if(!MatchPlayerState) return;

	MatchPlayerState->UpdatePlayerFolder(JSONFolder);
}


void AFDMatchPlayerController::UpdateMatchState_Implementation(bool bIsDefeat)
{
	auto GameHUD = GetHUD<AFDMatchGameHUD>();
	if (!GameHUD) return;
	GameHUD->UpdatePlayerMatchResult(bIsDefeat);
}

void AFDMatchPlayerController::OnPausePressed()
{
	auto GameHUD = GetHUD<AFDMatchGameHUD>();
	if (!GameHUD) return;

	 EHUDState CurrentState = GameHUD->GetHUDState();
	 if (CurrentState == EHUDState::PAUSE)
	 {
	 	if (PrevHUDState == EHUDState::NONE)
	 	{
	 		SetInputMode(FInputModeGameOnly());
	 		bShowMouseCursor = false;
	 	}
	 	GameHUD->SetState(PrevHUDState);
	 	PrevHUDState = CurrentState;
	 }
	 else
	 {
	 	SetInputMode(FInputModeGameAndUI());
	 	bShowMouseCursor = true;
	 	PrevHUDState = CurrentState;
	 	GameHUD->SetState(EHUDState::PAUSE);
	 }
}

void AFDMatchPlayerController::Disconnect_Implementation()
{
	if(!HasAuthority())
		GEngine->HandleDisconnect(GetWorld(), GetWorld()->GetNetDriver());
}
