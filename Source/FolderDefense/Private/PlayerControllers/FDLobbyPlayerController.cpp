// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/FDLobbyPlayerController.h"

void AFDLobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}

void AFDLobbyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AFDLobbyPlayerController::PreClientTravel(const FString& PendingURL, ETravelType TravelType,bool bIsSeamlessTravel)
{
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient)
	{
		ViewportClient->RemoveAllViewportWidgets();
	}
	Super::PreClientTravel(PendingURL, TravelType, bIsSeamlessTravel);
}

void AFDLobbyPlayerController::Disconnect_Implementation()
{
	if(!HasAuthority())
		GEngine->HandleDisconnect(GetWorld(), GetWorld()->GetNetDriver());
}
