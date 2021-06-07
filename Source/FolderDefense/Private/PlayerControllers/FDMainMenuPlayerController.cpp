// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/FDMainMenuPlayerController.h"

void AFDMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;
}
